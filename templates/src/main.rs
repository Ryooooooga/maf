use crate::files::token_h::TokenHTemplate;

pub mod files;
pub mod snippets;

fn main() -> anyhow::Result<()> {
    files::save_c_file("../include/maf/token.h", TokenHTemplate::new())?;

    Ok(())
}
