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
			PositionOffset	0 0 1
			pyrRotate	-90 0 0
		End
		Part	:Mana_Sparkles_02.part
						
	End

	Event
		EName	ManaMeros
		Type	Local
		At	Rotation
		BhvrOverride
			PositionOffset	0 -.3 0
			Scale		.5 .3 .9
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
			PositionOffset	0 -.4 0
			Scale		.5 .3 .9
			SpinJitter	0 .001 0
			Alpha		200
			StartColor	0 120 200
		End
		Geom	CosmicPeaceBringerShield
					
	End
End
###############################################################


End