resource "github_repository" "repository" {
  name        = "maf"
  visibility  = "public"
  description = ""

  has_issues             = true
  has_projects           = true
  allow_update_branch    = true
  delete_branch_on_merge = true
}

resource "github_repository_ruleset" "default_branch_protection" {
  name        = "default_branch_protection"
  repository  = github_repository.repository.name
  target      = "branch"
  enforcement = "active"

  conditions {
    ref_name {
      include = ["~DEFAULT_BRANCH"]
      exclude = []
    }
  }

  rules {
    creation                = true
    deletion                = true
    required_linear_history = true

    pull_request {
      required_review_thread_resolution = true
      allowed_merge_methods             = ["squash", "rebase"]
    }

    required_status_checks {
      strict_required_status_checks_policy = true

      dynamic "required_check" {
        for_each = flatten([
          for os in ["ubuntu-latest", "macos-latest"] : [
            for cc in ["gcc", "clang"] : [
              for build_type in ["Debug", "Release"] : "test-${os}-${cc}-${build_type}"
            ]
          ]
        ])
        content {
          context        = required_check.value
          integration_id = local.rulesets.status_check_integration_id.github_actions
        }
      }

      required_check {
        context        = "clang-format"
        integration_id = local.rulesets.status_check_integration_id.github_actions
      }
    }
  }
}
