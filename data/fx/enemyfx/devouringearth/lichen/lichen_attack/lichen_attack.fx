FxInfo

#########################################################
Lifespan  200

Condition
	On 	Time
	Time 	0
	
	Event
		EName 	OneFootRootNode
		Type	Local 
		At	Root
		GEOM	OneFootRootNode
		Sound lichen2 70 70 .65
		Sound lichen3 70 70 .55
		Sound lichen4 70 70 .65
		Sound lichen5 70 70 .6
	
	End

	Event
		EName 	Lichen_Attack
		Type	Local 
		At	OneFootRootNode
		AltPiv	1
		Part	:Lichen_Attack_Burst.part
		Part	:Lichen_Attack_Glow.part
		Lifespan 50
	End

End

Condition
	On 	Time
	Time 	20
	Event
		EName 	Lichen_Attack
		Type	Local 
		At	OneFootRootNode
		AltPiv	1
		Part	:Lichen_Attack_Poof_02.part
		Lifespan 50
	End

End

Condition
	On 	Time
	Time 	40
	Event
		EName 	Lichen_Attack
		Type	Local 
		At	OneFootRootNode
		AltPiv	1
		Part	:Lichen_Attack_Burst_02.part
		Lifespan 100
	End

	Event
		EName	Prime
		Type	Start
		At	OneFootRootNode
		AltPiv	1
		Part	:Lichen_Attack_01.part
		Part	:Lichen_Attack_02.part
		Bhvr	:Lichen_Spit.bhvr
		Magnet	Target
		LookAt	Target
	End
End

Condition
	On 	PrimeHit

	Event
		Ename 	Prime
		Type	Destroy
	End
End

End