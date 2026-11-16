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
				
		bhvr	:MirrorTeleportBubble2.bhvr

		Geom	FX_MoltenBronzeGlob_02
		Geom	FX_MoltenBronzeGlob_01

		
		Lifespan	30
	End

End


End
