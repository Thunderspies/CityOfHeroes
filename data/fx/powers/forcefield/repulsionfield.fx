#########################################################
##	forceBubble
########################################################
FxInfo

Input  
	Inpname	HIPS
End

Input  
	Inpname	Root
End

Input  
	Inpname	WepR
End

Input  
	Inpname	WepL
End

Input  
	Inpname	Origin
End


#####################################################################################


Condition
	On	Time	
	Time 	0

	Event
		Type 	Local
		At	Origin
		Sound forcehands6 80 80 .8
	End

End

Condition
	On	Time	
	Time 	10

	Event
		Type 	Local
		At	Origin
		Sound ForceField3_loop 100.0 100.0 .42
		bhvr	behaviors/soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 220
	End

End



Condition
	On	Time	
	Time 	10

	Event
		Type 	Local
		At	Origin
		Sound ForceField5_loop 100.0 100.0 .3
		bhvr	behaviors/soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 220
	End

End

Condition
	On 	Time
	Time 	10

	Event
		EName 	Prime
		Type	Posit
		At	WepR
		Part1	:HandGlows.part
				
	End
	
	Event
		EName 	Prime
		Type	Posit
		At	WepL
		Part1	:HandGlows.part
		#Sound	ForceField3_loop 80.0 30.0 .24
	End	

	Event
		EName 	Hookup
		Type	local
		At	Root
		Geom	RootToChestAdjustment
		#SOund	Forcefield5_loop 80 30 .19
		
	End

	
End

Condition
	On 	Time
	Time 	53

	Event
		EName 	Prime3
		Type	start
		At	WepR
	
		Geom	FX_ForceBubble
		Bhvr 	behaviors/ForceBubbleHands.bhvr

		LifeSpan	9

	End
	
	Event
		EName 	Prime4
		Type	start
		At	WepL
		
		Geom	FX_ForceBubble
		Bhvr 	behaviors/ForceBubbleHands.bhvr

		LifeSpan	9

	End

End

Condition
	On 	cycle
	Time 	35
	Chance	1
		
	
	Event
		EName 	Prime
		Type	local
		At	Hookup
		Altpiv	1
		Bhvr 	behaviors/powers/forcefield/ForceBombLarge2.bhvr
		
		Part1	powers/forcefield/BubbleSphereMelee.part
		Lifespan	1.2		
	End

	Event
		EName 	Prime2
		Type	local 
		At	Hookup
		Altpiv	1
		Bhvr 	behaviors/powers/forcefield/ForceBombLarge.bhvr
		Part1	:RepulsionRing3.part
		Geom	FX_ForceBubble
		Lifespan	7		
	End

End

Condition
	On 	Time
	Time 	20

	Event

		Ename	ForceBall
		Type	local
		At	Hookup
		Altpiv	1
		#Part1	:BubbleSphere.part
		Bhvr 	behaviors/powers/forcefield/ForceFieldBubble.bhvr
		geom	FX_ForceBubble
		LifeSpan	8
	End


	Event
		ENAME	BubbleHit01
		Type	local
		At	Hookup
		Altpiv	1
		Bhvr 	behaviors/powers/forcefield/ForceRepulsion01.bhvr
		
		Geom	ForceRepulsion01
				
	End

	Event
		ENAME	BubbleHit02
		Type	local
		At	Hookup
		Altpiv	1
		Bhvr 	behaviors/powers/forcefield/ForceRepulsion02.bhvr
		
		Geom	ForceRepulsion02
				
	End

	Event
		ENAME	BubbleHit03
		Type	local
		At	Hookup
		Altpiv	1
		Bhvr 	behaviors/powers/forcefield/ForceRepulsion03.bhvr
		
		Geom	ForceRepulsion03
				
	End

	Event
		ENAME	BubbleHit04
		Type	local
		At	Hookup
		Altpiv	1
		Bhvr 	behaviors/powers/forcefield/ForceRepulsion04.bhvr
		
		Geom	ForceRepulsion04
				
	End

End

Condition
	On 	Time
	Time 	23


	Event
		ENAME	BubbleHit01a
		Type	local
		At	Hookup
		Altpiv	1
		Bhvr 	behaviors/powers/forcefield/ForceRepulsion01a.bhvr
		
		Geom	ForceRepulsion01
				
	End
		
	Event
		ENAME	BubbleTrail01
		Type	local 
		At	BubbleHit01
		altpiv	1
		Part1	:ForceRepulsionTrail.part
				
	End

	Event
		ENAME	BubbleHit02a
		Type	local
		At	Hookup
		Altpiv	1
		Bhvr 	behaviors/powers/forcefield/ForceRepulsion02a.bhvr
		
		Geom	ForceRepulsion02
				
	End
	
	Event
		ENAME	BubbleTrail02
		Type	local 
		At	BubbleHit02
		altpiv	1
		Part1	:ForceRepulsionTrail.part
				
	End

	Event
		ENAME	BubbleHit03a
		Type	local
		At	Hookup
		Altpiv	1
		Bhvr 	behaviors/powers/forcefield/ForceRepulsion03a.bhvr
		
		Geom	ForceRepulsion03
				
	End

	Event
		ENAME	BubbleTrail03
		Type	local 
		At	BubbleHit03
		altpiv	1
		Part1	:ForceRepulsionTrail.part
				
	End

	Event
		ENAME	BubbleHit04a
		Type	local
		At	Hookup
		Altpiv	1
		Bhvr 	behaviors/powers/forcefield/ForceRepulsion04a.bhvr
		
		Geom	ForceRepulsion04
				
	End

	Event
		ENAME	BubbleTrail04
		Type	local 
		At	BubbleHit04
		altpiv	1
		Part1	:ForceRepulsionTrail.part
				
	End

End
#####################################################################################

Condition
	On 	Time
	Time 	27

	Event
		ENAME	BubbleHit01b
		Type	local
		At	Hookup
		Altpiv	1
		Bhvr 	behaviors/powers/forcefield/ForceRepulsion01b.bhvr
		
		Geom	ForceRepulsion01
				
	End

	Event
		ENAME	BubbleHit02b
		Type	local
		At	Hookup
		Altpiv	1
		Bhvr 	behaviors/powers/forcefield/ForceRepulsion02b.bhvr
		
		Geom	ForceRepulsion02
				
	End

	Event
		ENAME	BubbleHit03b
		Type	local
		At	Hookup
		Altpiv	1
		Bhvr 	behaviors/powers/forcefield/ForceRepulsion03b.bhvr
		
		Geom	ForceRepulsion03
				
	End

	Event
		ENAME	BubbleHit04b
		Type	local
		At	Hookup
		Altpiv	1
		Bhvr 	behaviors/powers/forcefield/ForceRepulsion04b.bhvr
		
		Geom	ForceRepulsion04
				
	End

End



Condition
	On 	Time
	Time 	38.5
Event
		ENAME	ForceBall
		Type	Destroy 
						
	End

End

Condition
	On 	Time
	Time 	33

	Event
		ENAME	BubbleHit01
		Type	Destroy 
						
	End

	Event
		ENAME	BubbleHit02
		Type	Destroy 
						
	End

	Event
		ENAME	BubbleHit03
		Type	Destroy 
						
	End

	Event
		ENAME	BubbleHit04
		Type	Destroy 
						
	End

	
End

Condition
	On 	Time
	Time 	28
	Event
		ENAME	BubbleHit01a
		Type	Destroy 
						
	End
	
	Event
		ENAME	BubbleHit02a
		Type	Destroy 
						
	End

	Event
		ENAME	BubbleHit03a
		Type	Destroy 
						
	End

	Event
		ENAME	BubbleHit04a
		Type	Destroy 
						
	End

	
End

	Condition
	On 	Time
	Time 	37
	Event
		ENAME	BubbleHit01b
		Type	Destroy 
						
	End

	Event
		ENAME	BubbleHit02b
		Type	Destroy 
						
	End

	Event
		ENAME	BubbleHit03b
		Type	Destroy 
						
	End

	Event
		ENAME	BubbleHit04b
		Type	Destroy 
						
	End

	
End


End
