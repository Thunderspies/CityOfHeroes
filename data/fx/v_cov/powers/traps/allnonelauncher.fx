#########################################################
##	
##
FxInfo

##########################################################

Condition

	Event
		Ename	Base
		Type	start 
		At	root

		BHVR	:AllNOneLauncherScale.bhvr
		geom	GrenadeLauncherBase
				
	End
		
	Event
		Ename	Top
		Type	Local 
		At	root
		BHVR	:AllNOneLauncherScale.bhvr
		geom	GrenadeLauncherTop

	End
	
		
	Event
		Ename	archPoints
		Type	Local 
		At	root
		BHVR	:AllNOneLauncherScale.bhvr
		geom	electricityoffset

	End
#
#	Event
#		Ename	archPoint1
#		Type	Local 
#		At	archPoints
#		altpiv	1
#
#		Part	:ElectricityArch.part
#		Part	:ElectricityArchWhite.part
#		Part	:ElectricityPop.part
#
#		PMagnet	Base
#
#	End
#
#	Event
#		Ename	archPoint2
#		Type	Local 
#		At	archPoints
#		altpiv	2
#
#		Part	:ElectricityArch.part
#		Part	:ElectricityArchWhite.part
#		Part	:ElectricityPop.part
#
#		PMagnet	Base
#	End
End

End			