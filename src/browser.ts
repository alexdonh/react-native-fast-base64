import { encode, decode } from '.';

(global as any).btoa = decode;
(global as any).atob = encode;

export const btoa = decode;
export const atob = encode;
