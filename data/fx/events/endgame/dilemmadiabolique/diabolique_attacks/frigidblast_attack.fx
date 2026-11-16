#
FxInfo

	LifeSpan 200


###########################################################


Condition
	On 	Time
	Time	10
	Event
		Type	Local
		At	Root
		Sound 	DD_FrigidBlast_Attack_01 300 100 .8
	End
End

	Condition
		On Time
		Time 10


		Event
			At WepR
			Type Local
			EName RightHandMist
			Part :FrigidLance_Hand_Glow.part
			Part :FrigidLance_Hand_Glow_Additive.part
			Part :FrigidLance_Hand_Snow.part
			Part :FrigidLance_Hand_Snow_Additive.part
			Part :FrigidLance_Hand_Core.part
			LifeSpan 37
		End

	End

	#Condition
		#On Time
		#Time 18

		#Event
			#At WepL
			#Type Start
			#Part :FrigidLance_Hand_Glow.part
			#Part :FrigidLance_Hand_Glow_Additive.part
			#Part :FrigidLance_Hand_Snow.part
			#Part :FrigidLance_Hand_Snow_Additive.part
			#Part :FrigidLance_Hand_Core.part
			#LifeSpan 37
		#End

	#End

	Condition
		On Time
		Time 17

		Event
			At WepL
			Type Start
			EName Prime
			LookAt Target
			Magnet Target
			Bhvr :FrigidBlast_Projectile.bhvr
			BhvrOverride
				StartColor 50 10 80
			End
			Geom Tintable_Dark_FX_IceBolt
		End

		Event
			At Prime
			Type Local
			EName Trail
			BhvrOverride
				PositionOffset 0.0 0.0 3.0
			End
			Part :FrigidBlast_Glow.part
			Part :FrigidBlast_Core.part
			Part :FrigidBlast_Glow_Additive.part
			Part :FrigidBlast_Snow.part
			Part :FrigidBlast_Snow_Additive.part
		End

	End

	Condition
		On PrimeHit

		Event
			Type Destroy
			EName Prime
		End

	End

End
