#############################################################
## AccelerateCombat.fx
#############################################################

FxInfo

#############################################################

Condition
	On	TriggerBits
	TriggerBits	NONHUMAN

	Event
		Type	Destroy
		EName	ALL
	End
End

#############################################################

Condition
	On	Time
	Time	1

	Event
		ENAME	Bubblies
		Type	Posit
		At	Emblem
		part1	:AccelerateCombatBubbles01.part
		part2	:AccelerateCombatFGlow01.part
		part3	:AccelerateCombatFGlow02.part
		Bhvr	Behaviors\GenericParticleFade.bhvr
		LifeSpan 55
	End
End

Condition
	On	Time
	Time	10

	Event
		ENAME	Bubblies
		Type	Posit
		At	Neck
		part1	:AccelerateCombatBubbles01.part
		part2	:AccelerateCombatGlow01.part
		part3	:AccelerateCombatGlow02.part
		part4	:AccelerateCombatGlowBig01.part
		pmagnet	Head
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Sound hasten2 60 60 0.75
		LifeSpan 30
	End
End

Condition
	On	Time
	Time	20

	Event
		ENAME	Bubblies
		Type	Posit
		At	Eyes
		part1	:AccelerateCombatBubbles01.part
		part2	:AccelerateCombatFGlow01.part
		part3	:AccelerateCombatFGlow02.part
		Bhvr	Behaviors\GenericParticleFade.bhvr
		LifeSpan 30
	End
End

Condition
	On	Time
	Time	25

	Event
		ENAME	Bubblies
		Type	Posit
		At	UarmR
		part1	:AccelerateCombatBubbles01.part
		part2	:AccelerateCombatFGlow01.part
		part3	:AccelerateCombatFGlow02.part
		part4	:AccelerateCombatLinkerGlow01.part
		Bhvr	Behaviors\GenericParticleFade.bhvr
		pmagnet	Head
		LifeSpan 15
	End

	Event
		ENAME	Bubblies
		Type	Posit
		At	UarmL
		part1	:AccelerateCombatBubbles01.part
		part2	:AccelerateCombatFGlow01.part
		part3	:AccelerateCombatFGlow02.part
		part4	:AccelerateCombatLinkerGlow01.part
		Bhvr	Behaviors\GenericParticleFade.bhvr
		pmagnet	Head
		LifeSpan 15
	End
End

Condition
	On	Time
	Time	30

	Event
		ENAME	Bubblies
		Type	Posit
		At	LarmR
		part1	:AccelerateCombatBubbles01.part
		part2	:AccelerateCombatFGlow01.part
		part3	:AccelerateCombatFGlow02.part
		part4	:AccelerateCombatLinkerGlow01.part
		Bhvr	Behaviors\GenericParticleFade.bhvr
		pmagnet	UarmR
		LifeSpan 15
	End

	Event
		ENAME	Bubblies
		Type	Posit
		At	LarmL
		part1	:AccelerateCombatBubbles01.part
		part2	:AccelerateCombatFGlow01.part
		part3	:AccelerateCombatFGlow02.part
		part4	:AccelerateCombatLinkerGlow01.part
		Bhvr	Behaviors\GenericParticleFade.bhvr
		pmagnet	UarmL
		LifeSpan 15
	End
End

Condition
	On	Time
	Time	35

	Event
		ENAME	Bubblies
		Type	Posit
		At	HandR
		part1	:AccelerateCombatBubbles01.part
		part2	:AccelerateCombatFGlow01.part
		part3	:AccelerateCombatFGlow02.part
		part4	:AccelerateCombatLinkerGlow01.part
		Bhvr	Behaviors\GenericParticleFade.bhvr
		pmagnet	LarmR
	End

	Event
		ENAME	Bubblies
		Type	Posit
		At	HandL
		part1	:AccelerateCombatBubbles01.part
		part2	:AccelerateCombatFGlow01.part
		part3	:AccelerateCombatFGlow02.part
		part4	:AccelerateCombatLinkerGlow01.part
		Bhvr	Behaviors\GenericParticleFade.bhvr
		pmagnet	LarmL
	End
End

#############################################################

End