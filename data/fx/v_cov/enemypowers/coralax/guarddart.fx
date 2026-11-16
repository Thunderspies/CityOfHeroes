#########################################################
##	Fireweapon
########################################################
FxInfo

Lifespan	400

########################################################################


Condition
	On 	Time
	Time 	19

	Event
		EName	Prime
		Type	start
		At	wepL
		Geom	QuillCoral01

#		Bhvr	:DartNOSpin.bhvr
		Bhvr	:Dart.bhvr
		part	:dartDust.part
		sound	Urchinthrow1 80 70 .7
		Magnet	Target
		LookAt	Target

			
	End

End

Condition
	On 	PrimeHit

	Event
		EName 	Prime
		Type	Destroy

	End

End

Condition
	On 	Prime1Hit
	
	Event
		EName 	Prime1
		Type	Destroy

	End


End

End
##################################

