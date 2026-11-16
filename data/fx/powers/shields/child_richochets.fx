#############################################################
## Child_Richochets.fx
#############################################################

FXInfo

## RICHOCETS #######################################################

Condition
	On 	cycle
	Time 	10
	Chance	0.125

	Event
		EName	3
		Type	Local
		At	LlegL
		part	POWERS\Invulnerability\FortitudeRaysSmall2.part
		part	POWERS\Invulnerability\FortitudeRing2.part
		lifespan 25
	End
End

Condition
	On 	cycle
	Time 	11
	Chance	0.125

	Event
		EName	3
		Type	Local
		At	LlegR
		part	POWERS\Invulnerability\FortitudeRaysSmall2.part
		part	POWERS\Invulnerability\FortitudeRing2.part
		lifespan 25
	End
End

Condition
	On 	cycle
	Time 	12
	Chance	0.125

	Event
		EName	3
		Type	Local
		At	LarmR
		part	POWERS\Invulnerability\FortitudeRaysSmall2.part
		part	POWERS\Invulnerability\FortitudeRing2.part
		lifespan 25
	End
End

Condition
	On 	cycle
	Time 	11
	Chance	0.125

	Event
		EName	3
		Type	Local
		At	LarmL
		part	POWERS\Invulnerability\FortitudeRaysSmall2.part
		part	POWERS\Invulnerability\FortitudeRing2.part
		lifespan 25
	End
End

Condition
	On 	cycle
	Time 	12
	Chance	0.125

	Event
		EName	3
		Type	Local
		At	UarmL
		part	POWERS\Invulnerability\FortitudeRaysSmall2.part
		part	POWERS\Invulnerability\FortitudeRing2.part
		lifespan 25
	End
End

Condition
	On 	cycle
	Time 	13
	Chance	0.125

	Event
		EName	3
		Type	Local
		At	UarmR
		part	POWERS\Invulnerability\FortitudeRaysSmall2.part
		part	POWERS\Invulnerability\FortitudeRing2.part
		lifespan 25
	End
End

#############################################################

End