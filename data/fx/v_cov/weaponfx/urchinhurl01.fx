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

		Bhvr	:UrchinNOSpin.bhvr
		Part1	:UrchinStreaks01.part
		sound	Urchinthrow1 80 70 .7
		Magnet	Target
		LookAt	Target

			
	End

	Event
		EName	Prime1
		Type	start
		At	wepL
		Geom	Corolax_Urchin

		Bhvr	:Urchin.bhvr

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

