#############################################################
## Death_Cold_DemonPrince.fx
#############################################################

FxInfo
Lifespan 180

#########################################################


Condition
	On	Time
	Time	20

	Event
		Type	PositOnly
		At	Head
		Part	:Death_Snow_Trail.part
		Lifespan	45
		LifespanJitter 	5
	End

	Event
		Type	PositOnly
		At	UArmL
		Part	:Death_Snow_Trail.part
		Lifespan	55
		LifespanJitter 	5
	End

	Event
		Type	PositOnly
		At	UArmR
		Part	:Death_Snow_Trail.part
		Lifespan	45
		LifespanJitter 	5
	End

	Event
		Type	PositOnly
		At	LArmL
		Part	:Death_Snow_Trail.part
		Lifespan	45
		LifespanJitter 	5
	End

	Event
		Type	PositOnly
		At	T_LArmR
		Part	:Death_Snow_Trail.part
		Lifespan	55
		LifespanJitter 	5
	End

	Event
		Type	PositOnly
		At	WepL
		Part	:Death_Snow_Trail.part
		Lifespan	55
		LifespanJitter 	5
	End

	Event
		Type	PositOnly
		At	WepR
		Part	:Death_Snow_Trail.part
		Lifespan	55
		LifespanJitter 	5
	End

	Event
		Type	PositOnly
		At	FootL
		Part	:Death_Snow_Trail.part
		Lifespan	55
		LifespanJitter 	5
	End

	Event
		Type	PositOnly
		At	FootR
		Part	:Death_Snow_Trail.part
		Lifespan	55
		LifespanJitter 	5
	End
End

Condition
	On	Time
	Time	40

	Event
		Type	StartPositOnly
		At	Root
		BhvrOverride
			PositionOffset	0.0 0.05 0
		End
		Part	:Cold_Mist_Origin.part
		Part	:Cold_Snow_Origin.part
		Part	:Cold_Snow_Origin_Flat.part
		Lifespan 50
	End
End

Condition
	On	Time
	Time	60

	Event
		Type	Local
		At	Root
		ChildFX	:Physics_Ice_Death.fx
	End

	Event
		Type	Local
		At	Hips
		Bhvr	behaviors/WarpOut_Fade.bhvr
		Flags 	AdoptParentEntity
		Part	:Explosion_Cold_Flash.part
		Part	:Explosion_Cold_Flash_Center.part
		Part	:Explosion_Cold_Streaks.part
		Part	:Explosion_Mist_Full.part
		Lifespan 3
	End
End

Condition
	On 	Time
	Time 	62

	Event
		Type	StartPositOnly
		At	Root
		BhvrOverride
			PositionOffset	0.0 1.5 0.0
		End
		Part	:Explosion_Cold_SparkCloud.part
		Part	:Explosion_Cold_Fire.part
		Part	:Explosion_Cold_Mist_Lingering.part
		Part	:Explosion_Cold_Sparks.part
		Lifespan 12
	End

End


##########################################################

End
