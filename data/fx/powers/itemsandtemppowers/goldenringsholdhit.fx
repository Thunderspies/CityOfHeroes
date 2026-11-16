#########################################################
##	ChillingTouchHands
FxInfo

LifeSpan		40

##################################



########################################################################
##################### Audio #######################################
########################################################################


Condition
	On	Time
	Time	0
	
	Event	
		Type	Local
		At	WepL
		Sound GoldenRings3 80 80 .89
						
	End

End


########################################################################
##################### Light Pulse #######################################
########################################################################

#Condition
#	On Cycle
#	Time 60
#	Event
#		EName 	Prime
#		Type	SetLight
#		Bhvr	:lightPulseclamp.bhvr
#		LifeSpan 60
#	End
#End

########################################################################
##################### Rings #######################################
########################################################################

Condition
	On	Time
	Time	0
	
	Event	
		Type	Local
		At	Root
		
		BhvrOverride
			PyrRotateJitter	25 360 25
			SpinJitter	0.0001 .01 0.0001
			InitialVelocity	0 -2 0	
			PositionOffset	0 23 0	
			Spin		0 .01 0
		End

		Geom	FX_GoldenRings
		LifeSpan	10
						
	End
End

Condition
	On	Time
	Time	2

	Event	
		Type	Local
		At	Root
		
		BhvrOverride
			PyrRotateJitter	25 360 25
			SpinJitter	0.0001 .01 0.0001
			InitialVelocity	0 -2 0	
			PositionOffset	0 23.25 0	
			Spin		0 .01 0
		End

		Geom	FX_GoldenRings
		LifeSpan	10
						
	End
End

Condition
	On	Time
	Time	4
	
	Event	
		Type	Local
		At	Root
		
		BhvrOverride
			PyrRotateJitter	25 360 25
			SpinJitter	0.0001 .01 0.0001
			InitialVelocity	0 -2 0	
			PositionOffset	0 23.5 0	
			Spin		0 .01 0
		End

		Geom	FX_GoldenRings
		LifeSpan	10
						
	End
End

Condition
	On	Time
	Time	7

	Event	
		Type	Local
		At	Root
		
		BhvrOverride
			PyrRotateJitter	25 360 25
			SpinJitter	0.0001 .01 0.0001
			InitialVelocity	0 -2 0	
			PositionOffset	0 23.75 0	
			Spin		0 .01 0
		End

		Geom	FX_GoldenRings
		LifeSpan	10
						
	End
End

Condition
	On	Time
	Time	9
	
	Event	
		Type	Local
		At	Root
		
		BhvrOverride
			PyrRotateJitter	25 360 25
			SpinJitter	0.0001 .01 0.0001
			InitialVelocity	0 -2 0	
			PositionOffset	0 24 0	
			Spin		0 .01 0
		End

		Geom	FX_GoldenRings
		LifeSpan	10
						
	End

End

########################################################################
##################### Rings Hit #######################################
########################################################################

Condition
	On	Time
	Time	6
	
	Event	
		Type	Local
		At	Chest

		Part	:RingFlashStreaks.part
		Part	:BrightEmberFlash.part
		LifeSpan	10
						
	End

End

Condition
	On	Time
	Time	11
	
	Event	
		Type	Local
		At	Chest

		Part	:BrightFlash.part
		Part	:RingFlashStreaks2.part
		Part	:BrightEmberFlash2.part
		Part	:RingFlashStreaks.part
		LifeSpan	10
						
	End

End

Condition
	On	Time
	Time	14
	
	Event	
		Type	Local
		At	Chest
		
		Part	:RingFlashStreaks2.part
		Part	:RingFlashStreaks.part
		Part	:RingFlashStreaks2.part
		Part	:RingFlashStreaks.part
		LifeSpan	10
						
	End

End

End			