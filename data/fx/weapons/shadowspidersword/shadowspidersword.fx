#########################################################
## Indigo's Sword
#########################################################

FxInfo
#########################################################

Flags    InheritAlpha

#########################################################
## Sword GEO
#########################################################

Condition
	On 	Time
	Time 	10

	Event
		EName 	Prime
		Type	Local 
		At	WepR
		Geom	GEO_WepR_ShadowSpider
		Sound swordshing 70 70 .6
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
	End

End

#########################################################
## Darkness FX
#########################################################

Condition
	On 	Time
	Time 	10

	Event
		EName 	Prime
		Type	Local 
		At	WepR
		
		BhvrOverride
			PositionOffset	0 0 1.3
		End

		Part1	:SwordBladeBase.part
		Part1	:SwordBladeBaseSubtractive.part
		Part2	:SwordBladeTendrils.part
		Part3	:SwordBladeTendrilsSubtractive.part
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
		#LifeSpan	30
	End
	
End

#########################################################

#Condition
#	On Time 
#	Time 28 #13
#
#	Event
#		EName	pow2
#		Type 	Destroy
#			
#	End
#
#End
#
#Condition
#	On Time 
#	Time 75
#
#	Event
#		EName	all
#		Type 	Destroy
#			
#	End
#
#End


#########################################################

End			