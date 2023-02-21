import React, { useEffect, useState } from 'react';
import { StyleSheet, View, TextInput } from 'react-native';
import { Buffer } from 'buffer';
import {
  encode,
  decode,
  fromByteArray,
  toByteArray,
} from 'react-native-fast-base64';

export default function App() {
  const [input, setInput] = useState('');
  const [output, setOutput] = useState('');
  const [currentFocus, setCurrentFocus] = useState<'input' | 'output'>('input');

  const onInputChange = (text: string) => {
    setInput(text);
  };

  const onOutputChange = (text: string) => {
    setOutput(text);
  };

  useEffect(() => {
    if (currentFocus === 'input') {
      setOutput(encode(input));
    }
  }, [input]); // eslint-disable-line react-hooks/exhaustive-deps

  useEffect(() => {
    if (currentFocus === 'output') {
      setInput(decode(output));
    }
  }, [output]); // eslint-disable-line react-hooks/exhaustive-deps

  return (
    <View style={styles.container}>
      {/* decode */}
      <View style={styles.textInputContainer}>
        <TextInput
          style={styles.textInput}
          value={input}
          onChangeText={onInputChange}
          onFocus={() => setCurrentFocus('input')}
          multiline={true}
        />
      </View>
      {/* encode */}
      <View style={styles.textInputContainer}>
        <TextInput
          style={styles.textInput}
          value={output}
          onChangeText={onOutputChange}
          onFocus={() => setCurrentFocus('output')}
          multiline={true}
        />
      </View>
      {/* fromByteArray */}
      <View style={styles.textInputContainer}>
        <TextInput
          style={styles.textInput}
          value={fromByteArray(Buffer.from(input))}
          editable={false}
          onFocus={() => setCurrentFocus('output')}
          multiline={true}
        />
      </View>
      {/* toByteArray */}
      <View style={styles.textInputContainer}>
        <TextInput
          style={styles.textInput}
          value={JSON.stringify(toByteArray(output))}
          editable={false}
          onFocus={() => setCurrentFocus('output')}
          multiline={true}
        />
      </View>
    </View>
  );
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
    alignItems: 'center',
    justifyContent: 'center',
    padding: 20,
  },
  textInputContainer: {
    flex: 1,
    flexDirection: 'row',
    margin: 20,
  },
  textInput: {
    textAlignVertical: 'top',
    flex: 1,
    borderWidth: 1,
    borderColor: '#000',
  },
});
