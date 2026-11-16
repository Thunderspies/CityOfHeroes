#########################################################
##	template

FxInfo

Lifespan 36


Condition
	On 	Time
	Time 	20

	Event
		Type	local
		At	Origin
		Sound clobberwhoosh 100.0 100.0 .78
	End

End

Condition
	On 	Time
	Time 	25

	Event
		EName	Streak
		Type	local
		At	Mystic
		Geom	swoosh
		bhvr	behaviors/Medieval1.bhvr
	End

End

################################################################################
##Streaks
################################################################

Condition
	On 	Time
	Time 	22

	Event
		EName	Prime
		Type	start
		At	FootR
		Bhvr	behaviors/Maceprojectile2.bhvr

		Part	:MaceRaysTall.part
		Part	:MaceRaysCenter1.part
		Part	:MaceRaysTall.part
		Part	:MaceRaysCenter1.part
		
		Part	:Embers.part
		Part	:MaceRaysSparks.part
		Magnet	Target
		LookAt	Target
	End

	Event
		EName	Prime1
		Type	start
		At	FootR
		Bhvr	behaviors/Maceprojectile2.bhvr
		
		Part	:MaceEnergyRingsFlat.part
		Part	:MaceEnergyRings3.part
		
		Magnet	T_Root
		LookAt	T_Root
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
	On 	Time
	Time 	40

	Event
		EName	all
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

	