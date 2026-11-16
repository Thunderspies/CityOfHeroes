#########################################################
## Black Dwarf Smite
#########################################################

FxInfo

LifeSpan	50

## SOUND FX #######################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Primea
		Type	Local 
		At	Eyes
		Sound Warshadeeyes3 60 60 .72
		Sound PeaceHeavyPunch 80 80 .9
	End
End

## HANDS #######################################################

Condition
	On 	Time
	Time 	6

	Event
		Type	Local
		At	wepR
		part	:HandGlows.part
		part	:HandGlowsPurple.part
		part	:HandGlowsBlue.part
		part	:Sparks.part
		LifeSpan	30
	End

	Event
		Type	Local
		At	wepL
		part	:HandGlows.part
		part	:HandGlowsPurple.part
		part	:HandGlowsBlue.part
		part	:Sparks.part
		LifeSpan	30
	End
End

## IMPACT FX #######################################################

Condition
	On 	Time
	Time 	25

	Event
		Type	Local 
		At	root
		
		Bhvr	:FootStompCameraShake.bhvr
		Part	:NovaTendrils.part
		Part	:NovaTendrilsPurple.part
		Part	:NovaTendrilsBLK.part
		Part	:NovaTendril.part
		Part	:NovaTendrilPurple.part
		Part	:NovaTendrilBlue.part
		Part	:NovaTendrilsFlat.part
		Part	:NovaTendrilsFlatBLK.part
		Part	:NovaTendrilsFlatBlue.part
		Part	:NovaTendrilFlat.part
		Part	:NovaTendrilFlatBlue.part
		Part	:NovaTendrilFlatPurple.part
		Part	:InfernorockBits2.part
		Part	:NovaTendrilBase.part
		Part	:NovaTendrilBaseBlue.part
		Part	:NovaTendrilBasePurple.part
		LifeSpan	25
	End
End

#########################################################

End		