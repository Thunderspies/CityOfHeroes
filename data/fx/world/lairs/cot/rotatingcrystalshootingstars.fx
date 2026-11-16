#########################################################
##	SStreetSteeam01
##
FxInfo
	
LifeSpan 0

Input  
	InpName	Origin
End


Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	Local 
		At	Origin
		Geom	FX_RuneStreak
		bhvr	behaviors/CrystalSpins.bhvr
		
	End

	Event
		EName 	1
		Type	Local 
		At	Prime
		 AltPiv	1
		
		Part1	:GlowOrbStar.part
		Part2	:GlowOrbTail.part
	
	End

End

Condition
	On 	Time
	Time 	0

	Event
		EName 	2
		Type	Local 
		At	Origin
		Geom	FX_RuneStreak
		bhvr	behaviors/CrystalSpinsOpposite.bhvr
		
	End

	Event
		EName 	3
		Type	Local 
		At	2
		 AltPiv	1
		
		Part1	:GlowOrbStar.part
		Part2	:GlowOrbTail.part
			
	End
End
######################################################

Condition
	On 	Time
	Time 	90

	Event
		EName 	Prime2
		Type	Local 
		At	Origin
		Geom	FX_RuneStreak
		bhvr	behaviors/CrystalSpins.bhvr
		
	End

	Event
		EName 	a
		Type	Local 
		At	Prime2
		 AltPiv	1
		Part1	:GlowOrbStar.part
		Part2	:GlowOrbTail.part
			
	End

End

Condition
	On 	Time
	Time 	90

	Event
		EName 	b
		Type	Local 
		At	Origin
		Geom	FX_RuneStreak
		bhvr	behaviors/CrystalSpinsOpposite.bhvr
		
	End

	Event
		EName 	c
		Type	Local 
		At	b
		 AltPiv	1
		Part1	:GlowOrbStar.part
		Part2	:GlowOrbTail.part
				
	End

End



Condition
	On 	Time
	Time 	180

		Event
		EName 	Prime3
		Type	Local 
		At	Origin
		Geom	FX_RuneStreak
		bhvr	behaviors/CrystalSpins.bhvr
		
	End

	Event
		EName 	11
		Type	Local 
		At	Prime3
		 AltPiv	1
		Part1	:GlowOrbStar.part
		Part2	:GlowOrbTail.part
			
	End

End

Condition
	On 	Time
	Time 	180

	Event
		EName 	12
		Type	Local 
		At	Origin
		Geom	FX_RuneStreak
		bhvr	behaviors/CrystalSpinsOpposite.bhvr
		
	End

	Event
		EName 	13
		Type	Local 
		At	12
		 AltPiv	1
		Part1	:GlowOrbStar.part
		Part2	:GlowOrbTail.part
				
	End

End



Condition
	On	Time
	Time	733
	
	
	Event	
		Ename   Prime
		Type	Destroy

	End

	Event	
		Ename   2
		Type	Destroy

	End


	Event	
		Ename   3
		Type	Destroy

	End
	
	Event	
		Ename   4
		Type	Destroy

	End
End

	
Condition
	On	Time
	Time	823
	
	
	Event	
		Ename   Prime2
		Type	Destroy

	End

	Event	
		Ename   b
		Type	Destroy

	End


	Event	
		Ename   c
		Type	Destroy

	End
	
	Event	
		Ename   d
		Type	Destroy

	End
End

Condition
	On	Time
	Time	913
	
	
	Event	
		Ename   Prime3
		Type	Destroy

	End

	Event	
		Ename   11
		Type	Destroy

	End


	Event	
		Ename   12
		Type	Destroy

	End
	
	Event	
		Ename   13
		Type	Destroy

	End
End

Condition
	On	Time
	Time	913


	Event	
		Ename   all
		Type	Destroy

	End
	

End

End