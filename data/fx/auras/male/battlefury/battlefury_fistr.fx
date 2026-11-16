#############################################################
## BattleFury.fx
#############################################################

FxInfo

###########################################################

Condition
	On Time
	Time 0

#	Event
#		At LarmL
#		Type PositOnly
#		LookAt HandL
#		Pother HandL
#		Part :Blobs_SM2.part
#	End

	Event
		At LarmR
		Type PositOnly
		LookAt HandR
		Pother HandR
		Part :Blobs_SM2.part
	End

#	Event
#		At HandL
#		Type PositOnly
#		LookAt LarmL
#		POther LarmL
#		Part Auras\Male\BattleFury\BlobGlow_SM.part
#		Part :Blobs_SM.part
#	End

	Event
		At HandR
		Type PositOnly
		LookAt LarmR
		POther LarmR
		Part Auras\Male\BattleFury\BlobGlow_SM.part
		Part :Blobs_SM.part
	End
#
#	Event
#		At WepL
#		Type PositOnly
#		Part Auras\Male\BattleFury\BlobGlow_SM.part
#		Part :Blobs_SM.part
#	End

	Event
		At WepR
		Type PositOnly
		Part Auras\Male\BattleFury\BlobGlow_SM.part
		Part :Blobs_SM.part
	End


##################################################################

End
