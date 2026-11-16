#############################################################
## Android_Plasma_ChargedBlast.fx
#############################################################

FxInfo
LifeSpan 120

#############################################################

Input
	InpName Hips
End

#############################################################
Condition
	On	Time
	Time	12

	Event
		EName	GunAnchor
		Type	Local
		At	LArmR
		BhvrOverride
			PositionOffset	2.2 0.1 -0.5
		End
		Sound WW_PlasmaRifleFullAutoBurst_Attack 200 200 .8
	End
End

Condition
	On 	Cycle
	Time 	4

	Event
		Type	Start
		At	GunAnchor
		ChildFX	:Child_PlasmaFullAuto_Single.fx
		Lifespan	20
	End
End

#############################################################
#
#Condition
#	On	PrimeHit
#
#	Event
#		EName	Prime
#		Type	Destroy
#	End
#End

#############################################################

End