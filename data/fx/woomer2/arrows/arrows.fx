#########################################################
##	Fire_Ball
##
FxInfo

LifeSpan 160

Condition

	Event
		EName   ASTART_OREINT
		Type	Start
		At		Root
		GEOM    FX_ICEWEAPON
	End
	Event
		EName   AEND_PREOREINT
		Type	Local
		At		ASTART_OREINT
		#GEOM    FX_ICEWEAPON
		bhvr	:arrowsflip.bhvr
	End
	Event
		EName   AEND_OREINT
		Type	Local
		At		AEND_PREOREINT
		GEOM    FX_FIREWEAPON
		#bhvr	:arrowsstart.bhvr
	End
#########################################

	Event
		EName   arrowstart1
		Type	Local
		At		ASTART_OREINT
		GEOM    FX_ICEWEAPON
		bhvr	:arrowsstart.bhvr
	End
#	Event
#		EName   arrowstart2
#		Type	Start
#		At		ASTART_OREINT
#		bhvr	:arrowsstart2.bhvr
#	End
#	Event
#		EName   arrowstart3
#		Type	Start
#		At		ASTART_OREINT
#		bhvr	:arrowsstart3.bhvr
#	End
#	Event
#		EName   arrowstart4
#		Type	Start
#		At		ASTART_OREINT
#		bhvr	:arrowsstart4.bhvr
#	End
#	Event
#		EName   arrowstart5
#		Type	Start
#		At		ASTART_OREINT
#		bhvr	:arrowsstart5.bhvr
#	End

########################################

	Event
		EName   arrowend1
		Type	Local
		At		AEND_OREINT
		GEOM    FX_FIREWEAPON
		bhvr	:arrowsstart.bhvr
	End
#	Event
##		EName   arrowend2
##		Type	Start
##		At		AEND_OREINT
##		bhvr	:arrowsstart2.bhvr
##	End
##	Event
##		EName   arrowend3
##		Type	Start
##		At		AEND_OREINT
##		bhvr	:arrowsstart3.bhvr
##	End
##	Event
##		EName   arrowend4
##		Type	Start
##		At		AEND_OREINT
##		bhvr	:arrowsstart4.bhvr
##	End
##	Event
##		EName   arrowend5
##		Type	Start
##		At		AEND_OREINT
##		bhvr	:arrowsstart5.bhvr
##	End

#######################################
	Event
		EName 	Prime
		Type	Start 
		At		arrowstart1
		Bhvr	:arrows.bhvr
		Geom	FX_ICEWEAPON
		LookAt  arrowend1
		Magnet  arrowend1
	End

#	Event
#		EName 	Prime
#		Type	Start 
#		At		arrowstart2
#		Bhvr	:arrows.bhvr
#		Geom	FX_ICEWEAPON
#		LookAt  arrowend2
#		Magnet  arrowend2
#	End
#
#	Event
#		EName 	Prime
#		Type	Start 
#		At		arrowstart3
#		Bhvr	:arrows.bhvr
#		Geom	FX_ICEWEAPON
#		LookAt  arrowend3
#		Magnet  arrowend3
#	End
#
#	Event
#		EName 	Prime
#		Type	Start 
#		At		arrowstart4
#		Bhvr	:arrows.bhvr
#		Geom	FX_ICEWEAPON
#		LookAt  arrowend4
#		Magnet  arrowend4
#	End
#
#	Event
#		EName 	Prime
#		Type	Start 
#		At		arrowstart5
#		Bhvr	:arrows.bhvr
#		Geom	FX_ICEWEAPON
#		LookAt  arrowend5
#		Magnet  arrowend5
#	End

End

End	


			