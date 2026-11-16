#########################################################
##	Arcane Battery 02
########################################################
FxInfo

##########################  Large Glitter #############################
Condition
	On 	Time
	Time 	0

	Event
		EName	Rotation
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0 .5 .4
			pyrRotate	-90 0 0
		End
		Part	:Mana_Sparkles_04.part
						
	End

	Event
		EName	ManaMeros
		Type	Local
		At	Rotation
		BhvrOverride
			PositionOffset	.1 -.05 .1
			Scale		.25 .1 .27
			SpinJitter	0 .001 0
			Alpha		255
			StartColor	50 50 255
		End
		Geom	EarthGraspHit
					
	End

	
	Event
		EName	ManaMeros
		Type	Local
		At	Rotation
		BhvrOverride
			PositionOffset	.1 -.05 .1
			Scale		.25 .1 .27
			SpinJitter	0 .001 0
			Alpha		200
			StartColor	0 120 200
		End
		Geom	CosmicPeaceBringerShield
					
	End
End
###############################################################


End