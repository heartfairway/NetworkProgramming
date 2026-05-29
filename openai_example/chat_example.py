#!/usr/bin/env python3
"""
OpenAI v1 Chat Completions interactive example.

Uses POST /v1/chat/completions via the official openai SDK.
Configure base_url, API key, model name, and system prompt in code, then chat.
"""

from __future__ import annotations

import sys

from openai import OpenAI

# --- Edit settings here ---
BASE_URL = "https://api.openai.com/v1"
API_KEY = "sk-your-api-key-here"
MODEL = "gpt-4o-mini"
SYSTEM_PROMPT = "You are a helpful assistant."
# --------------------------


def main() -> None:
    print("=== OpenAI v1 Chat Completions Example ===\n")

    if not API_KEY or API_KEY == "sk-your-api-key-here":
        print("Error: Set API_KEY in the source code.", file=sys.stderr)
        sys.exit(1)

    client = OpenAI(base_url=BASE_URL.rstrip("/"), api_key=API_KEY)
    messages: list[dict[str, str]] = [{"role": "system", "content": SYSTEM_PROMPT}]

    print(f"Model: {MODEL}")
    print("Chat started (type quit, exit, or q to end)\n")

    while True:
        try:
            user_input = input("You: ").strip()
        except (EOFError, KeyboardInterrupt):
            print("\nGoodbye.")
            break

        if not user_input:
            continue
        if user_input.lower() in ("quit", "exit", "q"):
            print("Goodbye.")
            break

        messages.append({"role": "user", "content": user_input})

        try:
            response = client.chat.completions.create(
                model=MODEL,
                messages=messages,
            )
        except Exception as exc:
            print(f"\nAPI error: {exc}\n", file=sys.stderr)
            messages.pop()
            continue

        choice = response.choices[0].message
        assistant_text = choice.content or ""
        print(f"\nAssistant: {assistant_text}\n")

        messages.append({"role": "assistant", "content": assistant_text})


if __name__ == "__main__":
    main()
