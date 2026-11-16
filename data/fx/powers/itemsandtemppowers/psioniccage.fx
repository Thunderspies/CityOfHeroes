#########################################################
##	Fire_Ball
##
FxInfo

Input  
	InpName	Hips
End

Input  
	InpName	Root
End    


Condition	
	On	Time
	Time	0

	Event	
		Type	Local
		At	Root
		Sound glow6_loop 60 60 .55
		End
End


Condition
	On 	Time
	Time 	0
			
	Event
		ENAME	Sparkles
		Type	posit
		At	Root
		Geom	RootToHipsAdjustment
		Part	:PsionicCageRings.part
		Sound psionic4 60 60 .66
		#LifeSpan		48

	End

	Event
		ENAME	Explosion
		Type	posit
		At	Root
		
		Part	:PsionicStreaksBase.part
		Part	:PsionicStreaksBase2.part
		Part	:PsionicBombflash.part
		Part	:PsionicBombflashflat.part
		
		LifeSpan		20

	End

	Event
		ENAME	Explosion
		Type	posit
		At	Root
		
		Part	:PsionicBombflashLarge.part
		
		
		LifeSpan		30

	End

	Event
		ENAME	Explosion
		Type	posit
		At	Root
		
		Part	:PsionicBombflashLargeInitial.part
	End

End

Condition
	On 	Time
	Time 	30

	Event
		Type	start
		At	Sparkles
		Altpiv	1
		Bhvr	Behaviors/Spin.bhvr
		Part	:BubbleGlows.part
		Part	:BubbleStars.part
	End

End


End			