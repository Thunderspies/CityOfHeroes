#############################################################
## ToxicFire_Aura.fx
#############################################################

FxInfo

#########################################################

Condition
	On 	Time
	Time 	0

#	Event
#		Type	PositOnly
#		At	Chest
#		Part	:Fire_Cold.part
#		POther	Neck
#	End

	Event
		Type	PositOnly
		At	UArmL
		Part	:Fire_Cold_Body.part
		POther	Neck
	End

	Event
		Type	PositOnly
		At	UArmR
		Part	:Fire_Cold_Body.part
		POther	Neck
	End

	Event
		Type	PositOnly
		At	LArmL
		Part	:Fire_Cold_Body.part
		POther	UArmL
	End

	Event
		Type	PositOnly
		At	LArmR
		Part	:Fire_Cold_Body.part
		POther	UArmR
	End

	Event
		Type	PositOnly
		At	HandL
		Part	:Fire_Cold_Body.part
		POther	LArmL
	End

	Event
		Type	PositOnly
		At	HandR
		Part	:Fire_Cold_Body.part
		POther	LArmR

	End

	Event
		Type	PositOnly
		At	ULegL
		Part	:Fire_Cold_Body.part
		POther	LLegL
	End

	Event
		Type	PositOnly
		At	ULegR
		Part	:Fire_Cold_Body.part
		POther	LLegR

	End
End

#########################################################

End