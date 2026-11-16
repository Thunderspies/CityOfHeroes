#############################################################
## BattleFury.fx
#############################################################

FxInfo

###########################################################

Condition
	On Time
	Time 0

	Event
		At LarmL
		Type PositOnly
		LookAt HandL
		Pother HandL
		Part :Blobs_SM2.part
	End

	Event
		At LarmR
		Type PositOnly
		LookAt HandR
		Pother HandR
		Part :Blobs_SM2.part
	End

	Event
		At HandL
		Type PositOnly
		LookAt LarmL
		POther LarmL
		Part Auras\Male\BattleFury\BlobGlow_SM.part
		Part :Blobs_SM.part
	End

	Event
		At HandR
		Type PositOnly
		LookAt LarmR
		POther LarmR
		Part Auras\Male\BattleFury\BlobGlow_SM.part
		Part :Blobs_SM.part
	End

	Event
		At WepL
		Type PositOnly
		Part Auras\Male\BattleFury\BlobGlow_SM.part
		Part :Blobs_SM.part
	End

	Event
		At WepR
		Type PositOnly
		Part Auras\Male\BattleFury\BlobGlow_SM.part
		Part :Blobs_SM.part
	End

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
			PositionOffset 0.01 0.010 -0.070
		End
		Part Auras\Male\BattleFury\EyeBlobs_L.part
	End

	Event
		AltPiv 2
		At EyeAnchor
		Type Local
		Bhvr behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset 0 0.01 -0.05
		End
		Part :EyeBlobs_R.part
	End

	Event
		Type	Local
		At	EyeAnchor
		AltPiv	1
		Bhvr	behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0.0 0.0 -.01
		End
		Part	:Eye_Glow.part
	End

	Event
		Type	Local
		At	EyeAnchor
		AltPiv	2
		Bhvr	behaviors\GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0.0 0.0 -.01
		End
		Part	:Eye_Glow.part
	End
End
##################################################################

End
