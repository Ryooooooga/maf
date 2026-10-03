resource "github_repository" "repository" {
  name        = "maf"
  visibility  = "public"
  description = ""

  has_issues             = true
  has_projects           = true
  allow_update_branch    = true
  delete_branch_on_merge = true
}
