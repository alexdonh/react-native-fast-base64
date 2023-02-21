import { NativeModules } from 'react-native';

// const LINKING_ERROR =
//   `The package 'react-native-fast-base64' doesn't seem to be linked. Make sure: \n\n` +
//   Platform.select({ ios: "- You have run 'pod install'\n", default: '' }) +
//   '- You rebuilt the app after installing the package\n' +
//   '- You are not using Expo Go\n';

type FastBase64EncodeFunc = (s: string) => string;
type FastBase64DecodeFunc = (s: string) => string;

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

declare const FastBase64Encode: FastBase64EncodeFunc;
declare const FastBase64Decode: FastBase64DecodeFunc;

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
