#########################################################
FxInfo

Condition
	On	TriggerBits
	TriggerBits	FLY
	DoMany		1

	Event
		EName 	JetL
		Type	local 
		At	Leg_L_Jet1

		While	FLY 
		Flags	OneAtATime
		
		BhvrOverride
			PYRrotate	0 0 5
		End

		Part1	:RocketBootsThrusterGlowBlue.part
		Part2	:RocketBootsThrusterCenterBlue.part
		Part3	:RocketBootsThrustercore.part
		Part4	:RocketBootsThrusterTrail.part
		Part5	:RocketBootsThrusterGases.part
		#Sound	RocketBoots2_loop 70 65 .3
	End

	Event
		EName 	JetR
		Type	local
		At	Leg_L_Jet2
		While	FLY 
		Flags	OneAtATime

		BhvrOverride
			PYRrotate	-30 0 0
		End

		Part2	:RocketBootsThrusterCenterBluesmall.part
		Part3	:RocketBootsThrustercoresmall.part
		Part4	:RocketBootsThrusterTrailsmall.part
		#Sound	glow7_loop 70 65 .66
		
	End

End


Condition
	On	TriggerBits
	TriggerBits	FLY
	DoMany		1

	Event
		EName 	JetL2
		Type	local 
		At	Leg_R_Jet1
		While	FLY 
		Flags	OneAtATime
		
		BhvrOverride
			PYRrotate	0 0 -5
		End
		
		Part1	:RocketBootsThrusterGlowBlue.part
		Part2	:RocketBootsThrusterCenterBlue.part
		Part3	:RocketBootsThrustercore.part
		Part4	:RocketBootsThrusterTrail.part
		Part5	:RocketBootsThrusterGases.part
		#Sound	RocketBoots2_loop 70 65 .3
	End

	Event
		EName 	JetR2
		Type	local
		At	Leg_R_Jet2
		While	FLY 
		Flags	OneAtATime

		
		BhvrOverride
			PYRrotate	-30 0 0
		End
				
		Part2	:RocketBootsThrusterCenterBluesmall.part
		Part3	:RocketBootsThrustercoresmall.part
		Part4	:RocketBootsThrusterTrailsmall.part

		#Sound	glow7_loop 70 65 .66
		
	End

End



End				
