#############################################################
## HEADER
#############################################################

FxInfo

LifeSpan 30

#############################################################

Condition
	On 	Time
	Time 	2

	Event
		EName 	HitFX
		Type	Start
		At	Chest
		Part	:Hit_Whip_Star_Large.part
		Part	:Hit_Whip_Fire.part
		Part	:Hit_Whip_Fire_Alpha.part
		Part	:Hit_Whip_Star_Large3.part
		Part	:Hit_Whip_Sparks.part
		Part	:Hit_Whip_Glow.part
		#Sound	Hit6 100.0 30.0 .9
	End
End

#############################################################

End