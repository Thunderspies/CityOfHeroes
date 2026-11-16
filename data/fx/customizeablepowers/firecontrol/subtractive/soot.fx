#############################################################
## Soot.fx
#############################################################

FxInfo

LifeSpan 200

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	InitialSwordFire
		Type	Local
		At	WepR
		Part1	:FireHandz.part
		Part1	:FireHandz_Alpha.part
		Sound Firehand_loop 100.0 100.0 .45
		LifeSpan	60

	End

	Event
		EName	InitialSwordFire1
		Type	Local
		At	WepL
		Part1	:FireHandz.part
		Part1	:FireHandz_Alpha.part
		Sound Ignite2 100.0 100.0 .8
		LifeSpan	60
	End
End

#############################################################

Condition
	On 	Time
	Time 	18

	Event
		EName 	Prime
		Type	Start
		At	WepR
		Bhvr	CustomizeablePowers\FireControl\slowprjct5.bhvr
		Magnet	Target
		LookAt	Target
		Part1	:Soot.part
		Part2	:Soot2.part
		Part3	:SootChunks.part
		Part4	:Sootspark.part
		Sound Sootthrow 80 80 .75
	End
End

#############################################################

Condition
	On	PrimeHit

	Event
		Ename	All
		Type	Destroy
	End

End

#############################################################

End


