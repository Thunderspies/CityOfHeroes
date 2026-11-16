#############################################################
## FX System Name
#############################################################

FxInfo

LifeSpan	120

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	Offset
		Type	Local 
		At	Root
		BhvrOverride
			PositionOffset	0 0 6
		End
		Sound fireexplo3 80 80 .86
		Sound fireexplo4 80 80 .86
		Sound fireexplo5 80 80 .9
	End

	Event
		EName	ExplosionRing
		Type	Local 
		At	Offset
		bhvr	:ExplosionRing1.bhvr
		Part1	:Explosion_FireRing1.part
		Part2	:Explosion_FireRing2.part
		#Part4	:Explosion_FireRingSparks1.part
		LifeSpan	30
	End

	Event
		Type	Local 
		At	Offset
		Part1	:Explosion_Fireball_Dark1.part
		Part2	:Explosion_Fireball_Bright1.part
		Part2	:Explosion_Fireball_Bright2.part
		Lifespan	5
	End

	Event
		Type	start 
		At	Offset
		Part	:Explosion_Flash1.part
		Part	:Explosion_Streaks1.part
		Part	:Explosion_Sparks1.part
		Lifespan	3
	End
End

## PARTICLE DEBRIS ###########################################################

#Condition
#	On 	Time
#	Time 	3
#
#	Event
#		Type	Local 
#		At	Origin
#		Part1	:Debris1.part
#		Part1	:Debris2.part
#		#Sound	FireExplo 100.0 30.0 .8	
#		LifeSpan	5
#	End
#End

## FIERY COMETS ###########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName   MetalDebris
		Type	Local
		At	Root
		ChildFX	:FireComets.fx
	End
End

## METAL DEBRIS ###########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName   MetalDebris
		Type	Local
		At	Root
		ChildFX	:MetalDebris.fx
	End
End

#############################################################

End