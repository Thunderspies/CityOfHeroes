#########################################################
##	template

FxInfo


Flags InheritAnimScale

#####  Hand Arrow   ##############################

Condition
	On	Time
	Time	15

	Event	
		Type	Local
		At	WepR
		Sound arrowout2 50 50 .25
	End
End

Condition
	On 	Time
	Time 	15

	Event
##		Ename	Prop
		Type	Local
		At	WepR
		Bhvr	:HandRotate.bhvr
		Geom	normalarrow
	End
End		

######## LensFlare  #############################
#Condition
#	On 	Time
#	Time 	63
#	Event
#		Type	Local
#		At	WepR
#		Bhvr	:ArrowGlowOffset.bhvr
#		
#		Part	:AimFlash.part
#		Part	:AimFlashCircle.part
#		Part	:AimFlashRing.part
#		Part	:AimFlashRing.part
#		Part	:AimFlashStars.part
#		Sound	arrowsniper2 50 40 .28
#		
#	End
#End


End		