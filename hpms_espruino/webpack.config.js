/* eslint-disable */

const path = require('path');
const webpack = require('webpack');
const ClosurePlugin = require('closure-webpack-plugin');

module.exports = {
  entry: './src/index.js',
  output: {
    filename: 'main.js',
    path: path.resolve(__dirname, 'dist'),
  },
  module: {
    rules: [
      {
        test: /\.js$/,
        exclude: /node_modules/,
        use: {
          loader: "babel-loader",
        }
      }
    ]
  },
  optimization: {
    minimizer: [
      new ClosurePlugin({mode: 'STANDARD'})
    ]
  }
};
