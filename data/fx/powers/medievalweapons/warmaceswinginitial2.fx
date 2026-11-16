#########################################################
##	template

FxInfo

LifeSpan	200

Condition
	On	Time
	Time	17

	Event	
		Type	Local
		At	Origin
		Sound Whooshbig3 100.0 100.0 .8

	End
End

Condition
	On 	Time
	Time 	25

	Event
		Type	local
		At	Origin
		Sound Whooshbig3 100.0 100.0 .8
	End

End


Condition
	On 	Time
	Time 	64

	Event
		EName	Streak
		Type	Start
		At	Mystic
		Geom	swoosh
		bhvr	behaviors/Medieval1.bhvr
		Sound Whoosh5 100.0 100.0 .6
		Lifespan 1
	End

End

#############################################################
##projectile
#################################################

Condition
	On 	Time
	Time 	60

		
	Event
		EName	Prime
		Type	start
		At	FootR	

		Part	:MaceRaysTall.part
		Part	:MaceRaysCenter1.part
		Part	:MaceRaysTall.part
		Part	:MaceRaysCenter1.part
		
		Part	:MaceRaysTall.part
		Part	:MaceRaysCenter1.part
		Part	:MaceRaysTall.part
		Part	:MaceRaysCenter1.part

		Part	:MaceEnergyRings.part
		#Part	:MaceEnergyRings3.part
		Bhvr	behaviors/MaceEnergyprojectile.bhvr
		
		Magnet	T_Root
		LookAt	T_Root

			
	End

	Event
		EName	Prime1
		Type	start
		At	FootR	

		Part	:MaceEnergyRingsFlat.part
		Part	:MaceEnergyRings3.part
		Bhvr	behaviors/MaceEnergyprojectile.bhvr
		
		Magnet	T_Root
		LookAt	T_Root

			
	End

End

Condition
	On 	Time
	Time 	61

	Event
		EName	Prime2
		Type	start
		At	FootR	

		Geom	BattleAxe

		Bhvr	behaviors/BattleAxePrjct1.bhvr
		
		Magnet	T_Root
		LookAt	T_Root

			
	End

	Event
		EName	Prime3
		Type	start
		At	FootR	

		Geom	BattleAxe

		Bhvr	behaviors/BattleAxePrjct1.bhvr
		
		Magnet	T_Root
		LookAt	T_Root

			
	End

	Event
		EName	Prime4
		Type	start
		At	FootR	

		Geom	BattleAxe

		Bhvr	behaviors/BattleAxePrjct1.bhvr
		
		Magnet	T_Root
		LookAt	T_Root

			
	End
End


Condition
	On 	Time
	Time 	41

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

Condition
	On 	Prime2Hit
	
	Event
		EName 	Prime2
		Type	Destroy

	End

End

Condition
	On 	Prime3Hit
	
	Event
		EName 	Prime3
		Type	Destroy

	End

End

Condition
	On 	Prime4Hit

	Event
		EName 	Prime4
		Type	Destroy

	End

End

Condition
	On 	Prime5Hit

	Event
		EName 	Prime5
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


	