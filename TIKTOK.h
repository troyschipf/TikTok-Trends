#include <iostream>
#include <string>
#ifndef TIKTOK_H_
#define TIKTOK_H_

class Tiktok{
public:

	// nested classes
	class AuthorMeta{
	public:
		AuthorMeta(std::string& aId, std::string& aNameA, std::string& aNickName, std::string& aVerified, std::string& aSignature, 
			std::string& abioLink, std::string& aAvatar, std::string& aPrivateAccount, std::string& aRegion, std::string& aFollowing, 
			std::string& aFans, std::string& aHeart, std::string& aVideo, std::string& aDigg);

		const std::string& getId() const {return id;}
		const std::string& getNameA() const {return name;}
		const std::string& getNickName() const {return nickName;}
		bool getVerified() const {return verified;}
		const std::string& getSignature() const {return signature;}
		const std::string& getBioLink() const {return bioLink;}
		const std::string& getAvatar() const {return avatar;}
		bool getPrivateAccount() const {return privateAccount;}
		const std::string& getRegion() const {return region;}
		const std::string& getFollowing() const {return following;}
		const std::string& getFans() const {return fans;}
		const std::string& getHeart() const {return heart;}
		const std::string& getVideo() const {return video;}
		const std::string& getDigg() const {return digg;}

	private:
		std::string id;
		std::string name;
		std::string nickName;
		bool verified;
		std::string signature;
		std::string bioLink;
		std::string avatar;
		bool privateAccount;
		std::string region;
		std::string following;
		std::string fans;
		std::string heart;
		std::string video;
		std::string digg;

	};
	class MusicMeta{
	public:
		MusicMeta(std::string& aMusicName, std::string& aMusicAuthor, std::string& aMusicOriginal, std::string& aPlayUrl, 
			std::string& aCoverMediumUrl, std::string& aMusicId);

		// getters
		const std::string& getMusicName() const {return musicName;}
		const std::string& getMusicAuthor() const {return musicAuthor;}
		const std::string& getPlayUrl() const {return playUrl;}
		const std::string& getCoverMediumUrl() const {return coverMediumUrl;}
		const std::string& getMusicId() const {return musicId;}

	private:
		std::string musicName;
		std::string musicAuthor;
		std::string musicOriginal;
		std::string playUrl;
		std::string coverMediumUrl;
		std::string musicId;

	};

	class VideoMeta{
	public:
		VideoMeta(std::string& aHeight, std::string& aWidth, std::string& aDuration, std::string& aCoverUrl, 
			std::string& aDefinition, std::string& aFormat,std::string& aDownloadAddr);

		// getters
		const std::string& getHeight() const {return height;}
		const std::string& getWidth() const {return width;}
		const std::string& getDuration() const {return duration;}
		const std::string& getCoverUrl() const {return coverUrl;}
		const std::string& getDefinition() const {return definition;}
		const std::string& getFormat() const {return format;}
		const std::string& getDownloadAddr() const {return downloadAddr;}
		
	private:
		std::string height;
		std::string width;
		std::string duration;
		std::string coverUrl;
		std::string definition;
		std::string format;
		std::string downloadAddr;


	};

	Tiktok(std::string& aID, std::string& aText, std::string& aCreateTime, std::string& aCreateTimeIso, AuthorMeta*& aAuthorMeta, 
		MusicMeta*& aMusicMeta,	std::string& aWebVideoUrl, VideoMeta*& aVideoMeta, std::string& aDiggCount, std::string& aShareCount, 
		std::string& aPlayCount, std::string& aCommentCount, std::string& aMentions);

	// getters
	const std::string& getID() const {return id;}
	const std::string& getText() const {return text;}
	const std::string& getCreateTime() const {return createTime;}
	const std::string& getCreateTimeIso() const {return createTimeIso;}
	AuthorMeta* getAuthorMeta() const {return authorMeta;}
	MusicMeta* getMusicMeta() const {return musicMeta;}
	const std::string& getWebVideoUrl() const {return webVideoUrl;}
	VideoMeta* getVideoMeta() const {return videoMeta;}
	const std::string& getDiggCount() const {return diggCount;}
	const std::string& getShareCount() const {return shareCount;}
	const std::string& getPlayCount() const {return playCount;}
	const std::string& getCommentCount() const {return commentCount;}
	const std::string& getMentions() const {return mentions;}


private:
	std::string id;
	std::string text;
	std::string createTime;
	std::string createTimeIso;
	AuthorMeta* authorMeta;
	MusicMeta* musicMeta;
	std::string webVideoUrl;
	VideoMeta* videoMeta;
	std::string diggCount;
	std::string shareCount;
	std::string playCount;
	std::string commentCount;
	std::string mentions;

};
#endif