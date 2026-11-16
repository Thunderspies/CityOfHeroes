#########################################################
## Staff of the M.A.G.I. - Defensive Aura
#########################################################

FxInfo

## SOUND FX #######################################################

Condition
	On 	Time
	Time 	15

	Event
		EName 	Thingy
		Type	local
		At	HandR
		Sound lightningfield2 80 80 .6
		LifeSpan	30
	End
End

Condition
	On 	Time
	Time 	40

	Event
		EName	lightning
		Type	local
		At	root
		Flags	PowerLoopingSound
		Sound elecArmor3_loop 70 70 .34
		bhvr	behaviors/soundFade3.bhvr
	End
End


## HAND FX #######################################################

Condition
	On 	Time
	Time 	15

	Event
		EName 	HandsCast
		Type	Local
		At	HandR
		Part1	:DefensiveAura\Aura_Hands_OutterGlow01.part
		Part1	:DefensiveAura\Aura_Hands_InnerGlow01.part
		Part1	:DefensiveAura\Aura_Hands_Lightning01.part
		Part1	:DefensiveAura\Aura_Hands_Lightning02.part
		Part2	:DefensiveAura\Aura_Hands_Lightning03.part
		Part3	:DefensiveAura\Aura_Hands_Ring01.part
		Part4	:DefensiveAura\Aura_Hands_SparkBurst01.part
		LifeSpan	45
	End
End

Condition
	On 	Time
	Time 	15

	Event
		EName 	HandsCast
		Type	Local
		At	HandL
		Part1	:DefensiveAura\Aura_Hands_OutterGlow01.part
		Part1	:DefensiveAura\Aura_Hands_InnerGlow01.part
		Part1	:DefensiveAura\Aura_Hands_Lightning01.part
		Part1	:DefensiveAura\Aura_Hands_Lightning02.part
		Part2	:DefensiveAura\Aura_Hands_Lightning03.part
		Part3	:DefensiveAura\Aura_Hands_Ring01.part
		Part4	:DefensiveAura\Aura_Hands_SparkBurst01.part
		LifeSpan	45
	End
End

## GROUND FX #######################################################

Condition
	On 	Time
	Time 	30

	Event
		EName	Aura
		Type	Local
		At	Root
		Part4	:DefensiveAura\Aura_Lightning_Arcs02.part
		Part5	:DefensiveAura\Aura_Lightning_Arcs01.part
		Part1	:DefensiveAura\Aura_Lightning_ArcsWhite01.part
		Part1	:DefensiveAura\Aura_Lightning_ArcsWhite02.part
	End
End

Condition
	On 	Time
	Time 	30

	Event
		EName	Ground
		Type	Local
		At	Root
		Part2	:DefensiveAura\Aura_Lightning_Arcs04.part
		Part3	:DefensiveAura\Aura_Lightning_Arcs03.part
		Part2	:DefensiveAura\Aura_Ring.part
		Part3	:DefensiveAura\Aura_ThinRing.part
		Part2	:DefensiveAura\Aura_GroundFlashes01.part
	End
End

#########################################################

End