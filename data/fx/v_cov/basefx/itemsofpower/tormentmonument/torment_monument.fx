#########################################################
##	Arcane Battery 01
########################################################
FxInfo

##########################  Large Glitter #############################
Condition
	On 	Time
	Time 	0

	Event
		EName	Center
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0 5.75 0
			#pyrRotate	-1.5708 0 0
		End
		Pmagnet	Center
		Part	:Penumbra.part
		Part	:PenumbraDark.part
		Part	:MagicSparkles.part
		Part	:LightRays.part
		Part	:RadiantPower.part
		Part	:GlowWisps.part
		Part	:ShadowWisps.part
						
	End


	Event
		EName	Base
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0 0 0
			#pyrRotate	-1.5708 0 0
		End
		Pmagnet	Center
		Part	:LightRays.part
						
	End



	Event
		EName	Theomeros01
		Type	Local
		At	Center
		BhvrOverride
			PositionOffset	0 0 0
			Scale		1.8 1.75 1.8
			SpinJitter	0 .001 0
			Alpha		175
			StartColor	155 110 55
		End
		Geom	EarthGraspHit
					
	End

	
	Event
		EName	Theomeros02
		Type	Local
		At	Center
		BhvrOverride
			PositionOffset	0 .5 0
			Scale		1.8 1.75 1.8
			SpinJitter	0 .001 0
			Alpha		100
			StartColor	160 20 125
		End
		Geom	CosmicPeaceBringerShield
					
	End

	
	Event
		EName	Theomeros03
		Type	Local
		At	Center
		BhvrOverride
			PositionOffset	0 0 0
			Scale		1.8 1.75 1.8
			SpinJitter	0 .001 0
			Alpha		255
			StartColor	155 75 200
		End
		Geom	FX_ForceBubble
					
	End

End
###############################################################


End