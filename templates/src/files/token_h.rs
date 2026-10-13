use askama::Template;

use crate::{
    files::token_h::Token::{Punctuator, Simple},
    snippets::c::{Enum, EnumTemplate, EnumToStringTemplate, Enumerator},
};

#[derive(Debug)]
enum Token<'a> {
    Simple { name: &'a str, text: &'a str },
    Punctuator { name: &'a str, text: &'a str },
    Keyword { name: &'a str },
}

impl From<&Token<'_>> for Enumerator {
    fn from(val: &Token<'_>) -> Self {
        match val {
            Simple { name, text } => Enumerator {
                name: name.to_string(),
                text: text.to_string(),
                comment: None,
            },
            Punctuator { name, text } => Enumerator {
                name: name.to_string(),
                text: text.to_string(),
                comment: Some(text.to_string()),
            },
            Token::Keyword { name } => Enumerator {
                name: name.to_string(),
                text: name.to_string(),
                comment: None,
            },
        }
    }
}

const TOKENS: &[Token] = &[
    Token::Simple {
        name: "eof",
        text: "end of file",
    },
    Token::Simple {
        name: "space",
        text: "space",
    },
    Token::Simple {
        name: "newline",
        text: "newline",
    },
    Token::Simple {
        name: "line_comment",
        text: "line comment",
    },
    Token::Simple {
        name: "identifier",
        text: "identifier",
    },
    Token::Simple {
        name: "integer",
        text: "integer",
    },
    Token::Simple {
        name: "string",
        text: "string",
    },
    Token::Punctuator {
        name: "plus",
        text: "+",
    },
    Token::Punctuator {
        name: "minus",
        text: "-",
    },
    Token::Keyword { name: "else" },
    Token::Keyword { name: "if" },
];

#[derive(Template)]
#[template(path = "files/include/maf/token.h.tmpl")]
pub struct TokenHTemplate<'a> {
    tokens: &'a [Token<'a>],
}

impl TokenHTemplate<'_> {
    pub fn new() -> Self {
        Self { tokens: TOKENS }
    }

    fn to_enum(&self) -> Enum {
        Enum {
            prefix: "maf_token".to_string(),
            variants: self.tokens.iter().map(|t| t.into()).collect(),
        }
    }

    fn render_enum(&self) -> anyhow::Result<String> {
        Ok(EnumTemplate {
            typ: self.to_enum(),
        }
        .render()?)
    }

    fn render_enum_to_string(&self) -> anyhow::Result<String> {
        Ok(EnumToStringTemplate {
            typ: self.to_enum(),
            arg: "kind".to_string(),
            unknown: "<UNKNOWN>".to_string(),
        }
        .render()?)
    }
}

impl Default for TokenHTemplate<'static> {
    fn default() -> Self {
        Self::new()
    }
}
