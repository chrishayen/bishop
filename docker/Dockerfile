FROM node:20-slim

RUN npm install -g @anthropic-ai/claude-code

RUN apt-get update && apt-get install -y git gh sudo jq curl && rm -rf /var/lib/apt/lists/*

RUN useradd -m -s /bin/bash agent && echo "agent ALL=(ALL) NOPASSWD:ALL" >> /etc/sudoers
RUN mkdir -p /home/agent/.claude /home/agent/.local/bin
RUN curl -sL https://github.com/Link-/gh-token/releases/download/v2.0.6/linux-amd64 -o /home/agent/.local/bin/gh-token && chmod +x /home/agent/.local/bin/gh-token
COPY CLAUDE.md /home/agent/.claude/CLAUDE.md
COPY .credentials.json /home/agent/.claude/.credentials.json
COPY app-private-key.pem /home/agent/app-private-key.pem
RUN chown -R agent:agent /home/agent

WORKDIR /workspace
RUN chown agent:agent /workspace

COPY entrypoint.sh /entrypoint.sh
RUN chmod 755 /entrypoint.sh

USER agent

ENTRYPOINT ["/entrypoint.sh"]
