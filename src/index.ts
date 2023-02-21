import { NativeModules } from 'react-native';

// const LINKING_ERROR =
//   `The package 'react-native-fast-base64' doesn't seem to be linked. Make sure: \n\n` +
//   Platform.select({ ios: "- You have run 'pod install'\n", default: '' }) +
//   '- You rebuilt the app after installing the package\n' +
//   '- You are not using Expo Go\n';

// const FastBase64 = NativeModules.FastBase64
//   ? NativeModules.FastBase64
//   : new Proxy(
//       {},
//       {
//         get() {
//           throw new Error(LINKING_ERROR);
//         },
//       }
//     );

declare const FastBase64FromArrayBuffer: (buf: ArrayBuffer) => string;
declare const FastBase64ToArrayBuffer: (s: string) => ArrayBuffer;
declare const FastBase64Encode: (s: string) => string;
declare const FastBase64Decode: (s: string) => string;

export function isLoaded() {
  return typeof FastBase64Encode === 'function';
}

if (!isLoaded()) {
  NativeModules.FastBase64?.install?.();
  if (!isLoaded()) {
    throw new Error(
      'JSI bindings were not installed for react-native-fast-base64 module'
    );
  }
}

export function encode(s: string) {
  return FastBase64Encode(s);
}

export function decode(s: string) {
  return FastBase64Decode(s);
}

export function fromByteArray(buf: Uint8Array) {
  if (buf.buffer.byteLength > buf.byteLength || buf.byteOffset > 0) {
    return FastBase64FromArrayBuffer(
      buf.buffer.slice(buf.byteOffset, buf.byteOffset + buf.byteLength)
    );
  }
  return FastBase64FromArrayBuffer(buf.buffer);
}

export function toByteArray(s: string) {
  return new Uint8Array(FastBase64ToArrayBuffer(s));
}
