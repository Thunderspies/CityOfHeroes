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
			Alpha		155
			StartColor	150 255 0
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
			Alpha		160
			StartColor	255 160 80
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
			Alpha		200
			StartColor	0 100 255
		End
		Geom	FX_ForceBubble
					
	End

End
###############################################################


End