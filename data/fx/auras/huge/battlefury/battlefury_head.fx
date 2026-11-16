#############################################################
## BattleFury.fx
#############################################################

FxInfo

###########################################################

Condition
	On Time
	Time 0

	Event
		At Chest
		Type PositOnly
		EName Core
	End

	Event
		At Head
		Type Local
		BhvrOverride
			PositionOffset 0 0.1 0
		End
		Part Auras\Male\BattleFury\Blob_Streaks.part
		Part :Blobs.part
	End

#	Event
#		At UArmL
#		Type PositOnly
#		Part :Blobs.part
#		Part Auras\Male\BattleFury\Blob_Streaks.part
#	End
#
#	Event
#		At UArmR
#		Type PositOnly
#		Part :Blobs.part
#		Part Auras\Male\BattleFury\Blob_Streaks.part
#	End
#
#	Event
#		At LArmL
#		Type PositOnly
#		LookAt UarmL
#		POther UarmL
#		Part Auras\Male\BattleFury\BlobGlow_SM.part
#		Part :Blobs_SM.part
#		Part :Blobs_SM2.part
#	End
#
#	Event
#		At LArmR
#		Type PositOnly
#		LookAt UarmR
#		POther UarmR
#		Part Auras\Male\BattleFury\BlobGlow_SM.part
#		Part :Blobs_SM.part
#		Part :Blobs_SM2.part
#	End
#
#	Event
#		At LarmL
#		Type PositOnly
#		LookAt HandL
#		Pother HandL
#		Part :Blobs_SM2.part
#	End
#
#	Event
#		At LarmR
#		Type PositOnly
#		LookAt HandR
#		Pother HandR
#		Part :Blobs_SM2.part
#	End
#
#	Event
#		At HandL
#		Type PositOnly
#		LookAt LarmL
#		POther LarmL
#		Part Auras\Male\BattleFury\BlobGlow_SM.part
#		Part :Blobs_SM.part
#	End
#
#	Event
#		At HandR
#		Type PositOnly
#		LookAt LarmR
#		POther LarmR
#		Part Auras\Male\BattleFury\BlobGlow_SM.part
#		Part :Blobs_SM.part
#	End
#
#	Event
#		At WepL
#		Type PositOnly
#		Part Auras\Male\BattleFury\BlobGlow_SM.part
#		Part :Blobs_SM.part
#	End
#
#	Event
#		At WepR
#		Type PositOnly
#		Part Auras\Male\BattleFury\BlobGlow_SM.part
#		Part :Blobs_SM.part
#	End
#
#	Event
#		At ULegL
#		Type PositOnly
#		EName LegHighL
#		Part :Blobs.part
#	End
#
#	Event
#		At ULegR
#		Type PositOnly
#		EName LegHighR
#		Part :Blobs.part
#	End
#
#	Event
#		At LLegL
#		Type PositOnly
#		EName LegLowL
#		LookAt LegHighL
#		POther LegHighL
#		Part Auras\Male\BattleFury\Blobs_Med.part
#	End
#
#	Event
#		At LlegL
#		Type PositOnly
#		EName LegLowL2
#		LookAt ULegL
#		POther ULegL
#		Part Auras\Male\BattleFury\Blobs_Med2.part
#	End
#
#	Event
#		At LLegR
#		Type PositOnly
#		EName LegLowR
#		LookAt LegHighR
#		POther LegHighR
#		Part Auras\Male\BattleFury\Blobs_Med.part
#	End
#
#	Event
#		At LlegR
#		Type PositOnly
#		EName LegLowL2
#		LookAt ULegR
#		POther ULegR
#		Part Auras\Male\BattleFury\Blobs_Med2.part
#	End
#
#	Event
#		At FootL
#		Type PositOnly
#		EName LegFootL
#		LookAt LegLowL
#		POther LegLowL
#		Part Auras\Male\BattleFury\BlobGlow_SM.part
#		Part :Blobs_SM.part
#		Part :Blobs_SM2.part
#	End
#
#	Event
#		At FootR
#		Type PositOnly
#		EName LegFootR
#		LookAt LegLowR
#		POther LegLowR
#		Part Auras\Male\BattleFury\BlobGlow_SM.part
#		Part :Blobs_SM.part
#		Part :Blobs_SM2.part
#	End

### eyes

	Event
		At Eyes
		Type Local
		EName EyeAnchor
		Bhvr behaviors\GenericParticleFade.bhvr
		Geom CircleOfThorns
	End

	Event
		AltPiv 1
		At EyeAnchor
		Type Local
		Bhvr behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset 0.0 -0.06 -0.125
		End
		Part Auras\Male\BattleFury\EyeBlobs_L.part
	End

	Event
		AltPiv 2
		At EyeAnchor
		Type Local
		Bhvr behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset 0.0 -0.06 -0.125
		End
		Part :EyeBlobs_R.part
	End

	Event
		Type	Local
		At	EyeAnchor
		AltPiv	1
		Bhvr	behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0.0 -0.06 -0.125
		End
		Part	:Eye_Glow.part
	End

	Event
		Type	Local
		At	EyeAnchor
		AltPiv	2
		Bhvr	behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0.0 -0.06 -0.125
		End
		Part	:Eye_Glow.part
	End
End
##################################################################

End
