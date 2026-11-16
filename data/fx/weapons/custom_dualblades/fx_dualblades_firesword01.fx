#########################################################
## FX Geo - Talsorian Blade Right
#########################################################

FxInfo

Flags InheritAlpha DontSuppress

#########################################################

Condition
        On	Time
        Time	0

#	Event
#		Type	Local
#		At	Origin
#		Bhvr	Behaviors\GenericParticleFade.bhvr
#		BhvrOverride
#			PositionOffset	0.4 0.0 -0.625
#			PyrRotate	-90 0 -5
#		End
#		Part	:TalsorianBlades_Glow01.part
#	End

#	Event
#		EName	Offset_Start
#		Type	Local
#		At	Offset
#		AltPiv	2
#		Part	Customizeablepowers/icemelee/snowfall.part
#		Part	Customizeablepowers/icemelee/snowfallWhips.part
#		Part	Customizeablepowers/icemelee/SwordSparklies.part
#		POther	Offset_End
#	End

#	Event
#		EName	Offset_End
#		Type	Local
#		At	Origin
#		BhvrOverride
#			PositionOffset	0 0 0
#			PyrRotate	90 0 0
#		End
#		AltPiv	1
#		Part	POWERS/ColdControl/GreatericeswordFrost.part
#		Part1	POWERS/ColdControl/Greatericesword2.part
#		Part5	POWERS/ColdControl/GreaterSwordtrail2.part
#		POther	Offset_Start
#	End

	Event
		Type	Local
		At	Origin
		BhvrOverride
			PositionOffset	0 0 2.5
		End
		Part1	Powers\FireControl\fireweapon3followa.part
		Part3	Powers\FireControl\fireweaponspark.part
	End

	Event
		Type	Local
		At	Origin
		Part1	Powers\FireControl\fireweapon3followa.part
		Part3	Powers\FireControl\fireweaponspark.part
	End

	Event
		Type	Local
		At	Origin
		Part1	Powers\FireControl\fireweapon1a.part
		Part2	Powers\FireControl\fireweapon2a.part
		Part3	Powers\FireControl\fireweapon3a.part
		#Part4	:fireweaponWhitness.part
		Part5	Powers\FireControl\fireweaponBlueGlow.part
	End
End

#########################################################

End