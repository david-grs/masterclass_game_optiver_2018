#pragma once

#include "execution_client.h"
#include "information_client.h"
#include "udp_client.h"

#include <string>
#include <vector>
#include <utility>
#include <map>

class Autotrader : public IUDPClientHandler
{
public:
	void Run();

private:
	// [IUDPClientHandler]
	void OnMulticastMessage(Address, std::string) override;

	// execution
	void OnOrderAck(std::string, Price, Volume);
	void OnPriceFeed(std::string, Price, Volume, Price, Volume);
	void OnTrade(std::string, std::string, Volume);
	void PrintPnl();

	// pricing
	double CalcVWAPChange(TopLevel, Side, Volume);
	double CalcVWAPPrediction(TopLevel, Side, Volume);

	void SubscribeToPrices();

	static constexpr uint16_t InfoServerPort = 7001;
	static constexpr uint16_t ExecServerPort = 8001;

	InformationClient mInformationClient{LOCAL_INFO_PORT, "REMOTE_ADDRESS", InfoServerPort, *this};
	ExecutionClient mExecutionClient{LOCAL_EXEC_PORT, "REMOTE_ADDRESS", ExecServerPort, *this};

	std::map<std::string /*instrument feedcode*/, TopLevel> mLastBook;

	std::vector<std::pair<Price, Volume>> mESXTrades;
	std::vector<std::pair<Price, Volume>> mSPTrades;
};
