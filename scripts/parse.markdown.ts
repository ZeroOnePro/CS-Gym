import fs from "fs";

const image_block = /!\[(.*?)]\((https?:\/\/\S+\.\w+)\)/gim;

const markdown = fs.readFileSync("./test.md", "utf-8");

const matches: RegExpMatchArray | null = markdown.match(image_block);

const extract_link = matches?.map((m) =>
  m.replace(/!\[Untitled]\(/, "").slice(0, -1)
);

export default extract_link || [];
