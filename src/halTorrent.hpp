
#pragma once

#include <string>
#include <vector>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/fstream.hpp>

using boost::filesystem::path;

namespace libtorrent { struct peer_info; }

namespace halite 
{

struct torrentBriefDetail 
{
	std::wstring filename;
	std::wstring status;
	std::pair<float,float> speed;
	float completion;
	int peers;
	int seeds;
};

class TorrentDetail 
{
public:
	TorrentDetail(std::wstring f, std::wstring s, std::wstring cT, std::pair<float,float> sp=std::pair<float,float>(0,0),
			float c=0, float d=0, boost::int64_t tWD=0, boost::int64_t tW=0, int p=0, int sd=0, boost::posix_time::time_duration eta=boost::posix_time::seconds(0)) :
		filename_(f),
		state_(s),
		currentTracker_(cT),
		speed_(sp),
		completion_(c),
		distributed_copies_(d),
		totalWantedDone_(tWD),
		totalWanted_(tW),
		peers_(p),
		seeds_(sd),
		estimatedTimeLeft_(eta)
	{}		

	TorrentDetail() {};
	
	const std::wstring& filename() const { return filename_; }
	const std::wstring& state() const { return state_; }
	const std::wstring& currentTracker() { return currentTracker_; }
	
	std::pair<float,float> speed() const { return speed_; }
	const float& completion() const { return completion_; }
	const float& distributedCopies() const { return distributed_copies_; }
	
	const boost::int64_t& totalWantedDone() const { return totalWantedDone_; }
	const boost::int64_t& totalWanted() const { return totalWanted_; }
	
	const int& peers() const { return peers_; }
	const int& seeds() const { return seeds_; }
	
	const boost::posix_time::time_duration& estimatedTimeLeft() { return estimatedTimeLeft_; }

public:
	std::wstring filename_;
	std::wstring state_;
	std::wstring currentTracker_;

	std::pair<float,float> speed_;		
	float completion_;	
	float distributed_copies_;
	
	boost::int64_t totalWantedDone_;
	boost::int64_t totalWanted_;
	
	int peers_;
	int seeds_;
	
	boost::posix_time::time_duration estimatedTimeLeft_;
};

typedef shared_ptr<TorrentDetail> TorrentDetail_ptr;
typedef std::vector<TorrentDetail_ptr> TorrentDetails;

struct PeerDetail 
{
	PeerDetail(const std::wstring& ip_address) :
		ipAddress(ip_address)
	{}
	PeerDetail(libtorrent::peer_info& peerInfo);
	
	bool operator==(const PeerDetail& peer) const
	{
		return (ipAddress == peer.ipAddress);
	}
	
	bool operator<(const PeerDetail& peer) const
	{
		return (ipAddress < peer.ipAddress);
	}
	
	wstring ipAddress;
	std::pair<float,float> speed;
	bool seed;
	std::wstring client;
};

typedef shared_ptr<PeerDetail> PeerDetail_ptr;
typedef std::vector<PeerDetail> PeerDetails;

class BitTorrent_impl;

class BitTorrent
{
public:
	void shutDownSession();
	
	bool listenOn(std::pair<int, int> const& portRange);
	int isListeningOn();
	void stopListening();
	
	void setSessionLimits(int maxConn, int maxUpload);
	void setSessionSpeed(float download, float upload);
	std::pair<double, double> sessionSpeed();
	
	void newTorrent(boost::filesystem::path filename, boost::filesystem::path files);
	void addTorrent(boost::filesystem::path file, path saveDirectory);
	void getAllTorrentDetails(TorrentDetails& torrentsContainer);
	TorrentDetail_ptr getTorrentDetails(std::string filename);
	
	void getAllPeerDetails(std::string filename, PeerDetails& peerContainer);
	
	void resumeAll();
	void closeAll();
	
	bool isTorrent(std::string filename);
	
	void pauseTorrent(std::string filename);
	void resumeTorrent(std::string filename);
	bool isTorrentPaused(std::string filename);
	
	void pauseAllTorrents();
	void resumeAllTorrents();
	
	void removeTorrent(std::string filename);
	void reannounceTorrent(std::string filename);
	
	void setTorrentLimit(std::string filename, int maxConn, int maxUpload);
	void setTorrentSpeed(std::string filename, float download, float upload);
	pair<int, int> getTorrentLimit(std::string filename);
	pair<float, float> getTorrentSpeed(std::string filename);
	
	friend BitTorrent& bittorrent();
	
private:
	BitTorrent();
	
	boost::scoped_ptr<BitTorrent_impl> pimpl;
};

BitTorrent& bittorrent();

};
