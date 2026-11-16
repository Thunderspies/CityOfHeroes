#########################################################
##	ChillingTouchHands
FxInfo

#LifeSpan		25

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
		Sound GoldenRings_Loop 80 80 .33
						
	End

End

########################################################################
##################### Light Pulse #######################################
########################################################################

Condition
	On Cycle
	Time 60
	Event
		EName 	Prime
		Type	SetLight
		Bhvr	:lightPulseclamp.bhvr
		LifeSpan 60
	End
End

########################################################################
##################### Rings #######################################
########################################################################

Condition
	On	Time
	Time	11
	
	Event	
		Type	Local
		At	Root
		
		Bhvr	behaviors/ParticleFade.bhvr
		BhvrOverride
			PyrRotateJitter	15 360 15
			PositionOffset	0 3 0	
			Spin		0 .01 0
			SpinJitter	0.005 .1 0.005
		End

		Geom	FX_GoldenRings
						
	End

End

Condition
	On	Time
	Time	12

	Event	
		Type	Local
		At	Root
		
		Bhvr	behaviors/ParticleFade.bhvr
		
		BhvrOverride
			PyrRotateJitter	15 360 15
			PositionOffset	0 3.25 0	
			Spin		0 .01 0
			SpinJitter	0.005 .1 0.005
		End

		Geom	FX_GoldenRings
						
	End

End

Condition
	On	Time
	Time	13
	
	Event	
		Type	Local
		At	Root
		
		Bhvr	behaviors/ParticleFade.bhvr
		
		BhvrOverride
			PyrRotateJitter	15 360 15
			PositionOffset	0 3.5 0	
			Spin		0 .01 0
			SpinJitter	0.005 .1 0.005
		End

		Geom	FX_GoldenRings
						
	End

End

Condition
	On	Time
	Time	14

	Event	
		Type	Local
		At	Root
		
		Bhvr	behaviors/ParticleFade.bhvr
		
		BhvrOverride
			PyrRotateJitter	15 360 15
			PositionOffset	0 3.75 0	
			Spin		0 .01 0
			SpinJitter	0.005 .1 0.005
		End

		Geom	FX_GoldenRings
						
	End

End

Condition
	On	Time
	Time	15
	
	Event	
		Type	Local
		At	Root
		
		Bhvr	behaviors/ParticleFade.bhvr
		
		BhvrOverride
			PyrRotateJitter	15 360 15
			PositionOffset	0 4 0	
			Spin		0 .01 0
			SpinJitter	0.005 .1 0.005
		End

		Geom	FX_GoldenRings
						
	End

End
	
End			