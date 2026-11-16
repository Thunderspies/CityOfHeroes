#############################################################
## ToxicFire_Aura.fx
#############################################################

FxInfo
Lifespan 70

#########################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	local
		At	WepR
		Sound firering 60.0 60.0 .7


	End
End


Condition
	On 	Time
	Time 	8

	Event
		Type	PositOnly
		At	WepL
		Part	:Hand_Fire_Fire.part
		Part	:Sparks_Fire.part
	End

End

#########################################################

End