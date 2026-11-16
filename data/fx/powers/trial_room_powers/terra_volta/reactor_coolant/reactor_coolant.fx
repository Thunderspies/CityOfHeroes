#########################################################
##	Fireweapon
########################################################
FxInfo

Lifespan	200

########################################################################


Condition
	On 	Time
	Time 	19

	Event
		EName	Prime
		Type	Start
		At	WepR
		Geom	GEO_WepR_Reactor_Coolant
		Bhvr	:Ring.bhvr
		Magnet	Target
		LookAt	Target
	End

		Event
		EName	RightRingFX
		Type	Local
		At	Prime
		Part	:BlueOuterGlow.part
		Part	:BlueTrailGlow.part
	End


	Event
		EName	RightRingFX
		Type	Local
		At	Prime
	End

End



Condition
	On 	Time
	Time 	14.5

	Event
		EName	Streak
		Type	local
		At	Mystic
		Geom	swoosh
		bhvr	behaviors/Claws.bhvr
		Sound throw1 50.0 50.0 1.0
	End

End



Condition
	On 	Time
	Time 	17.5
	
	Event
		EName	streak
		Type	Destroy
		
	End
	
End

Condition
	On 	PrimeHit

	Event
		EName 	Prime
		Type	Destroy

	End

End

End
##################################

