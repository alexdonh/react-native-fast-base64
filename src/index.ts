import { NativeModules } from 'react-native';

declare const FastBase64: {
  encode: (s: string) => string;
  decode: (s: string) => string;
  fromArrayBuffer: (buf: ArrayBuffer) => string;
  toArrayBuffer: (s: string) => ArrayBuffer;
};

export function isLoaded() {
  return typeof FastBase64 !== 'undefined';
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
  return FastBase64.encode(s);
}

export function decode(s: string) {
  return FastBase64.decode(s);
}

export function fromByteArray(buf: Uint8Array) {
  if (buf.buffer.byteLength > buf.byteLength || buf.byteOffset > 0) {
    return FastBase64.fromArrayBuffer(
      buf.buffer.slice(buf.byteOffset, buf.byteOffset + buf.byteLength)
    );
  }
  return FastBase64.fromArrayBuffer(buf.buffer);
}

export function toByteArray(s: string) {
  return new Uint8Array(FastBase64.toArrayBuffer(s));
}
