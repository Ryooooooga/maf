use askama::Template;

mod filters {
    #[askama::filter_fn]
    pub fn string_literal<T: std::fmt::Display>(
        s: T,
        _env: &dyn askama::Values,
    ) -> askama::Result<String> {
        let s = s.to_string();
        Ok(format!("{:?}", s))
    }
}

#[derive(Debug)]
pub struct Enum {
    pub prefix: String,
    pub variants: Vec<Enumerator>,
}

impl Enum {
    pub fn type_name(&self) -> String {
        format!("{}_t", self.prefix)
    }
}

#[derive(Debug)]
pub struct Enumerator {
    pub name: String,
    pub comment: Option<String>,
    pub text: String,
}

#[derive(Debug, Template)]
#[template(path = "snippets/c/enum.c.tmpl")]
pub struct EnumTemplate {
    pub typ: Enum,
}

#[derive(Debug, Template)]
#[template(path = "snippets/c/enum_to_string.c.tmpl")]
pub struct EnumToStringTemplate {
    pub typ: Enum,
    pub arg: String,
    pub unknown: String,
}

#[cfg(test)]
mod tests {
    use super::*;

    fn color_enum() -> Enum {
        Enum {
            prefix: "color".to_string(),
            variants: vec![
                Enumerator {
                    name: "red".to_string(),
                    comment: Some("#ff0000".to_string()),
                    text: "RED".to_string(),
                },
                Enumerator {
                    name: "green".to_string(),
                    comment: Some("#00ff00".to_string()),
                    text: "GREEN".to_string(),
                },
                Enumerator {
                    name: "blue".to_string(),
                    comment: Some("#0000ff".to_string()),
                    text: "BLUE".to_string(),
                },
                Enumerator {
                    name: "black".to_string(),
                    comment: None,
                    text: "BLACK".to_string(),
                },
            ],
        }
    }

    #[test]
    fn test_enum() {
        insta::assert_snapshot!(EnumTemplate { typ: color_enum() }.render().unwrap());
    }

    #[test]
    fn test_enum_to_string() {
        insta::assert_snapshot!(
            EnumToStringTemplate {
                typ: color_enum(),
                arg: "color".to_string(),
                unknown: "UNKNWON".to_string(),
            }
            .render()
            .unwrap()
        );
    }
}
