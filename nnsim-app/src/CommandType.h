#pragma once
enum CommandType
{
	/*
	 * net.AddLayer
	 * net.AddLayer
	 */
	NETWORK_STRUCTURE = 1,
	/*
	 * net.Evaluate(input[n])
	 */
	NETWORK_VALUE,
	/*
	 * input[n] = { x1, x2, x3, x4, ... xn }
	 * ideal[n] = { y1, y2, y3, y4, ... yn }
	 */
	DATA,
	/*
	 * net.input = input[i]
	 */
	NETWORK_INPUT,
	/*
	 * painter.clear
	 */
	NETWORK_CLEAR,
	INVALID_COMMAND,
	VISUALIZATION_WINDOW,
	TRAINER_ITERATION,
	TRAINER_EPOCH,
	TRAINER_TRAIN_EPOCH,
	TRAINER_TRAIN_LAST_EPOCH,
	TRAINER_TRAIN_ITERATION,
	TRAINER_CHECKTHRESHOLD,
	/*
	 trainer.Train(epochsCount)
	 */
	TRAINER_TRAIN,
	ASYNC_COMMAND,
	IGNORE_CMD,
	TEST
};