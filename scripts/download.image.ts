import axios from "axios";
import fs from "fs";
import extract_link from "./parse.markdown";

const getImage = async (url: string) => {
  const { data } = await axios.get(url);
  return data;
};

(async () => {
  const images = await axios.all(
    extract_link.map((link: string) => getImage(link))
  );

  images.map((image, index: number) =>
    image.pipe(fs.createWriteStream(__dirname + `../images/${index + 1}.png`))
  );
})();

/**
 * 노션 이미지는 1시간 동안만 유효해서 지금 당장은 직접 다운로드 받는 수 밖에 없는 것  같다
 * 일단 이미지 다운로드 기능하면서 파일 전송 원리 알게됨
 * browser에는 blob객체를 사용하지만, node.js에서는 stream으로 받을 수 있음
 * https://developer-alle.tistory.com/m/435
 * https://futurestud.io/tutorials/download-files-images-with-axios-in-node-js
 * 매번 노션 이미지를 Public url로 바꾸려하니 귀찮아 죽겠는데 이 문제 어떻게 해결할까?
 */
