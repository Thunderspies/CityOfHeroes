#########################################################
##	chill touch hit
FxInfo

LifeSpan	100

Input  
	InpName	WepR
End

Input  
	InpName	WepL
End

Input  
	InpName	Root
End

##################################
Condition
	On	Time
	Time	0
	
	Event
		EName	RightHand
		Type	start
		At	WepR
		
		Bhvr	behaviors/LightDummy.bhvr
		geom	LightDummy
		
	End

	Event
		EName	RBomb
		Type	posit
		At	RightHand
		Altpiv 1
		Part1	:DoublesBomb.part
	End
	
End

Condition
	On	Time
	Time	0

	Event
		EName	LeftHand
		Type	start
		At	WepL
		
		geom	LightDummy
		Bhvr	behaviors/LightDummy.bhvr
	End

	Event
		EName	LBomb
		Type	posit
		At	LeftHand
		
		Altpiv	1
		Part1	:DoublesBomb.part
	End

End	

Condition
	On	Time
	Time	10

	Event

		Ename	Mushroom
		Type	Start
		At	LBomb
		ChildFx POWERS/LightControl/BombHit.fx

	End

	Event

		Ename	Mushroom
		Type	Start
		At	RBomb
		ChildFx POWERS/LightControl/BombHit.fx

	End

End


End				