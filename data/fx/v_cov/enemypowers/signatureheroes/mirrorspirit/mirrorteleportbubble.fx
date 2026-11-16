#########################################################
##	Molten Bronze Splash
########################################################
FxInfo

############################################

Lifespan	500

############  Glob   ###################

Condition
	On 	Time
	Time 	0

	Event
		Ename	glob
		Type	Start
		At	Origin
		BhvrOverride
			PositionOffset		0 0.25 0
			pyrRotateJitter		0 180 0

		End
		
	End

	Event
		Ename	slob
		Type	Local
		At	glob
		
		
		BhvrOverride
			PositionOffset		0 0 1
		End

	End

	Event
		Ename	blob
		Type	StartPositOnly
		At	slob
		
		
		bhvr	:MirrorTeleportBubble.bhvr
		BhvrOverride
			pyrRotateJitter		180 180 180
		End
		Magnet	glob

		Geom	FX_MoltenBronzeGlob_02
		Geom	FX_MoltenBronzeGlob_01

		Lifespan	35
	End

End


End
