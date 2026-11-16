#########################################################
##	Arcane Battery 01
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
			PositionOffset	0 1.6 1.6
			pyrRotate	-90 0 0
		End
		Part	:Mana_Sparkles_01.part
						
	End

	Event
		EName	ManaMeros
		Type	Local
		At	Rotation
		BhvrOverride
			PositionOffset	0 -.4 0
			Scale		1.33 .33 1.33
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
			Scale		1.33 .33 1.33
			SpinJitter	0 .001 0
			Alpha		120
			StartColor	0 120 200
		End
		Geom	CosmicPeaceBringerShield
					
	End
End
###############################################################


End