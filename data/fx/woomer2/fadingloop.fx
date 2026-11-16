#########################################################
##	Fire_Ball
##
FxInfo

Lifespan 1000

Condition

	Event	
		Type	Local
		At		SPADR
		EName	MonkeyAnim
		Anim	Monkey
	End

	Event
		EName	MonkeyHead
		Type	Local
		At		MonkeyAnim
		AnimPiv	Head
#		Part	generic/fire.part
	End

	Event
		Type	Posit
		At		Head
		Magnet	MonkeyHead
#		Geom	LaserBeam
		Bhvr	:beamStretch.bhr
	End

End	


			