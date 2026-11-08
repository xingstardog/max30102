import BlockType from '../utils/block-type';
import DataType from '../utils/data-type';
import blockIconURI from './icon/blockIcon.svg';
import menuIconURI from './icon/menuIcon.svg';
import Func from './func';
import { setLocaleData, formatMessage, setLocale } from '../utils/translation';
import LocaleData from './locales';

setLocaleData(LocaleData);

class ExtensionMAX30102 {
    constructor(runtime, extensionId) {
        this.runtime = runtime;
        this.funcs = new Func(runtime, extensionId);
    }

    setLocale(locale) {
        setLocale(locale);
    }

    getCodePrimitives() {
        return this.funcs;
    }

    getInfo() {
        return {
            name: formatMessage({
                id: 'gui.extensionName.max30102',
                default: 'MAX30102'
            }),
            blockIconURI: blockIconURI,
            menuIconURI: menuIconURI,
            color1: '#FF6347',
            color2: '#D9503A',
            color3: '#B34030',
            blocks: [
                {
                    opcode: 'init',
                    blockType: BlockType.COMMAND,
                    text: formatMessage({
                        id: 'gui.blocklyText.max30102.init',
                        description: 'block msg for init',
                        default: 'initialize MAX30102'
                    })
                },
                {
                    opcode: 'isReady',
                    blockType: BlockType.BOOLEAN,
                    text: formatMessage({
                        id: 'gui.blocklyText.max30102.isReady',
                        description: 'block msg for isReady',
                        default: 'sensor connected?'
                    }),
                    outputTypes: [DataType.BOOL]
                },
                {
                    opcode: 'update',
                    blockType: BlockType.COMMAND,
                    text: formatMessage({
                        id: 'gui.blocklyText.max30102.update',
                        description: 'block msg for update',
                        default: 'update heart rate and SpO2 data'
                    })
                },
                {
                    opcode: 'getHR',
                    blockType: BlockType.REPORTER,
                    text: formatMessage({
                        id: 'gui.blocklyText.max30102.getHR',
                        description: 'block msg for getHR',
                        default: 'heart rate (BPM)'
                    }),
                    outputTypes: [DataType.INT32_T]
                },
                {
                    opcode: 'getSPO2',
                    blockType: BlockType.REPORTER,
                    text: formatMessage({
                        id: 'gui.blocklyText.max30102.getSPO2',
                        description: 'block msg for getSPO2',
                        default: 'blood oxygen (SpO2)'
                    }),
                    outputTypes: [DataType.INT32_T]
                },
                {
                    opcode: 'getTemp',
                    blockType: BlockType.REPORTER,
                    text: formatMessage({
                        id: 'gui.blocklyText.max30102.getTemp',
                        description: 'block msg for getTemp',
                        default: 'sensor temperature (℃)'
                    }),
                    outputTypes: [DataType.FLOAT]
                },
                {
                    opcode: 'stop',
                    blockType: BlockType.COMMAND,
                    text: formatMessage({
                        id: 'gui.blocklyText.max30102.stop',
                        description: 'block msg for stop',
                        default: 'stop collecting'
                    })
                }
            ],
            menus: {}
        };
    }
}

export default ExtensionMAX30102;
