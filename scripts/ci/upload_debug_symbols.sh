#!/usr/bin/env bash

((${BASH_VERSION%%.*} >= 4)) || { echo >&2 "$0: Error: Please upgrade Bash."; exit 1; }

set -euxo pipefail

# download sentry-cli
# TODO: currently this script downloads binaries and install them
# each time job is started, workarounds?
curl -sL https://sentry.io/get-cli/ | bash

#Where debug symbols will go
UPLOAD_URL=https://${SENTRY_HOST}/api/${SENTRY_PROJECT}/minidump/?sentry_key=${SENTRY_DSN_KEY}

SYMBOLS=$(find debug | xargs)

${INSTALL_DIR}/sentry-cli --auth-token ${SENTRY_AUTH_TOKEN} --url ${UPLOAD_URL} \
    --org ${SENTRY_ORG_SLUG}} \
    --project ${SENTRY_PROJECT_SLUG} ${SYMBOLS}