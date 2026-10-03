terraform {
  required_version = "1.16.5"

  cloud {
    organization = "Ryooooooga"

    workspaces {
      project = "github"
      name    = "maf"
    }
  }

  required_providers {
    github = {
      source  = "integrations/github"
      version = "~> 6.0"
    }
  }
}
